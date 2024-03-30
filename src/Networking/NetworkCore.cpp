#include <WinSock2.h>
#include <WS2tcpip.h>
#include "NetworkCore.h"
#include "../Engine/Engine.h"
#include "ConnectionManager.h"
#include "PacketManager.h"
#include "ByteStream.h"

NetworkCore *NetworkCore::instance = nullptr;

NetworkCore::NetworkCore() {
    instance = this;
    Engine::Get()->RegisterSystem<ConnectionManager>();
    Engine::Get()->RegisterSystem<PacketManager>();
}

void NetworkCore::PreUpdate(float deltaTime) {
    ReceiveIncomingPackets();
    ProcessIncomingPackets();
}

void NetworkCore::PostUpdate(float deltaTime) {
    SendOutgoingPackets();
}

void NetworkCore::SendPacket(const SharedPtr<IClientConnection> &client, std::unique_ptr<Packet> packet) {
    if (client->IsLocalServer()) {
        std::lock_guard<std::mutex> lock(instance->incomingPacketsMutex);
        instance->incomingPackets.push(MakeUnique<PendingPacket>(client, std::move(packet)));
    } else {
        std::lock_guard<std::mutex> lock(instance->outgoingPacketsMutex);
        instance->outgoingPackets.push(MakeUnique<PendingPacket>(client, std::move(packet)));
    }
}

bool NetworkCore::SendPacketImmediate(const SharedPtr<IClientConnection> &client, std::unique_ptr<Packet> packet) {
    if (client->IsLocalServer()) {
        SendPacket(client, std::move(packet));
        return true;
    }

    // We must be a remote client if we're not a server (dedicated or self-hosted).
    auto remoteClient = std::dynamic_pointer_cast<ClientConnection>(client);
    if (!remoteClient) {
        LOG_ERROR("NetworkCore", "Failed to cast client to ClientConnection.");
        return false;
    }

    // Serialize the packet.
    packet->type = PacketManager::GetType(packet.get());
    packet->sender = remoteClient->GetId();
    if (packet->type == 0) {
        LOG_ERROR("NetworkCore", "Failed to get packet type.");
        return false;
    }

    ByteStream stream;
    packet->Serialize(stream);

    // Convert the client addr to a sockaddr_in.
    auto remoteAddr = remoteClient->GetAddr();
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(remoteAddr.port);
    if (inet_pton(addr.sin_family, remoteAddr.ip.c_str(), &addr.sin_addr) != 1) {
        LOG_ERROR("NetworkCore", "Failed to convert IP address to sockaddr.");
        return false;
    }

    // Send the packet to the client.
    int result = sendto(networkSocket, stream.GetData(), stream.GetSize(), 0, (sockaddr *) &addr, sizeof(addr));
    if (result != SOCKET_ERROR) {
        LOG_DEBUG("NetworkCore", "Sent packet %hu to client %hu (%s:%d)", packet->type, remoteClient->GetId(), remoteAddr.ip.c_str(), remoteAddr.port);
        return true;
    }

    // Error handling.
    int errorCode = WSAGetLastError();
    switch (errorCode) {
        case WSAEWOULDBLOCK: {
            LOG_DEBUG("NetworkCore", "Temporary network error. Cannot send packet.");
            // Temporary error. Try again next tick.
            SendPacket(remoteClient, std::move(packet));
        }
            break;
        case WSAENETDOWN:
        case WSAEHOSTDOWN:
        case WSAENETUNREACH:
        case WSAEHOSTUNREACH:
        case WSAECONNRESET:
        case WSAECONNABORTED:
        case WSAECONNREFUSED:
        case WSAETIMEDOUT:
        case WSAESHUTDOWN:
        case WSAENOTCONN: {
            LOG_ERROR("NetworkCore", "Permanent network error. Cannot send packet.");
        }
            break;
        default: {
            // Windows error bullshit.
            LPVOID msgBuffer;
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &msgBuffer,
                0,
                nullptr
            );
            LOG_ERROR("NetworkCore", "Failed to send packet: %s", (char *) msgBuffer);
            LocalFree(msgBuffer);
        }
            break;
    }

    return false;
}

void NetworkCore::ReceiveIncomingPackets() {
    static char buffer[NETWORK_MSG_BUFFER_SIZE];
    int n = MAX_INCOMING_PACKETS_PER_FRAME;

    while (n-- > 0) {
        // Receive the packet.
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        int bytesReceived = recvfrom(networkSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (bytesReceived == SOCKET_ERROR || bytesReceived < MIN_PACKET_SIZE) {
            if (bytesReceived > 0) {
                LOG_WARN("NetworkCore", "Received packet is too small.");
            }

            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                // No more packets pending.
                break;
            }

            // Windows bullshit to get the error message.
            LPVOID msgBuffer;
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                error,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &msgBuffer,
                0,
                nullptr
            );
            LOG_ERROR("NetworkCore", "Failed to receive packet: %s", (char *) msgBuffer);
            LocalFree(msgBuffer);
            continue;
        }

        // Get the IP address of the sender.
        char ipString[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipString, sizeof(ipString)) == nullptr) {
            char errorString[128];
            strerror_s(errorString, sizeof(errorString), errno);
            LOG_ERROR("NetworkCore", "Failed to convert IP address to string: %s", errorString);
            continue;
        }

        // Get the packet type.
        ByteStream stream(buffer, bytesReceived);
        uint16_t type;
        stream.Read(type);

        // Deserialize the packet.
        auto packet = PacketManager::Deserialize(type, stream);
        if (packet == nullptr) {
            LOG_ERROR("NetworkCore", "Failed to deserialize packet of type %hu from %s:%d", type, ipString, ntohs(clientAddr.sin_port));
            continue;
        }

        // Push it into the queue for processing.
        auto client = ConnectionManager::GetOrCreateClient(packet->sender, {ipString, ntohs(clientAddr.sin_port)});
        {
            std::lock_guard<std::mutex> lock(incomingPacketsMutex);
            incomingPackets.push(MakeUnique<PendingPacket>(client, std::move(packet)));
        }
    }
}

void NetworkCore::ProcessIncomingPackets() {
    std::lock_guard<std::mutex> lock(incomingPacketsMutex);
    if (incomingPackets.empty()) {
        return;
    }

    int n = MAX_PROCESSING_PACKETS_PER_FRAME;
    while (!incomingPackets.empty() && n-- > 0) {
        UniquePtr<PendingPacket> pendingPacket = std::move(incomingPackets.front());
        PacketManager::Dispatch(pendingPacket->client, std::move(pendingPacket->packet));
        incomingPackets.pop();
    }

    if (!incomingPackets.empty()) {
        LOG_WARN("NetworkCore", "Incoming packet queue is getting large (%d).", incomingPackets.size());
    }
}

void NetworkCore::SendOutgoingPackets() {
    std::lock_guard<std::mutex> lock(outgoingPacketsMutex);
    if (outgoingPackets.empty()) {
        return;
    }

    int n = MAX_OUTGOING_PACKETS_PER_FRAME;
    while (!outgoingPackets.empty() && n-- > 0) {
        auto packet = std::move(outgoingPackets.front());
        SendPacketImmediate(packet->client, std::move(packet->packet));
        outgoingPackets.pop();
    }

    if (!outgoingPackets.empty()) {
        LOG_WARN("NetworkCore", "Outgoing packet queue is getting large (%d).", outgoingPackets.size());
    }
}
