#include <WinSock2.h>
#include <WS2tcpip.h>
#include "ClientNetworkInterface.h"
#include "Packets/ClientConnectPacket.h"
#include "Packets/ClientDisconnectPacket.h"

ClientNetworkInterface::~ClientNetworkInterface() {
    if (handshakeThread.joinable()) {
        handshakeThread.join();
    }
}

int ClientNetworkInterface::Init() {
    WSADATA wsaData;
    networkAddress = IPAddress(NETWORK_IP, NETWORK_PORT);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return WSAGetLastError();
    }

    if ((networkSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        return WSAGetLastError();
    }

    // Set blocking to non-blocking
    unsigned long nonBlocking = 1;
    int result = ioctlsocket(networkSocket, FIONBIO, &nonBlocking);
    if (result != NO_ERROR) {
        return result;
    }

    // connect
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(NETWORK_PORT);
    if (inet_pton(addr.sin_family, NETWORK_IP, &addr.sin_addr) != 1) {
        LOG_ERROR("Client", "Failed to convert IP address to sockaddr.");
        return -1;
    }

    if (connect(networkSocket, (sockaddr *) &addr, sizeof(addr)) != 0) {
        LOG_ERROR("Client", "Failed to connect to server.");
        return -1;
    }

    LOG_DEBUG("Client", "Client started on port %d", NETWORK_PORT);
    ConnectionManager::SetIsClient();
    ConnectionManager::SetServer({NETWORK_IP, NETWORK_PORT});

    return 0;
}

void ClientNetworkInterface::Start() {
    handshakeThread = std::thread(&ClientNetworkInterface::DoHandshake, this);
}

void ClientNetworkInterface::Shutdown() {
    if (isReady) {
        SendPacketImmediate<ClientDisconnectPacket>(ConnectionManager::GetServer(), networkId);
    }
}

void ClientNetworkInterface::OnHandshakeCompleted(uint32_t netId) {
    networkId = netId;
    isReady = true;

    LOG_DEBUG("Client", "DoHandshake completed (NetID: %u).", networkId);
}

void ClientNetworkInterface::DoHandshake() {
    if (isReady) {
        LOG_WARN("Client", "DoHandshake has already been performed.");
        return;
    }

    int secondsUntilHandshake = HANDSHAKE_TIMEOUT;
    float handshakeAttempt = 0;
    while (handshakeAttempt++ < HANDSHAKE_MAX_ATTEMPTS && !isReady) {
        LOG_DEBUG("Client", "Sending handshake packet. Attempt %d/%d", (int) handshakeAttempt, HANDSHAKE_MAX_ATTEMPTS);
        SendPacket<ClientConnectPacket>(ConnectionManager::GetServer());
        std::this_thread::sleep_for(std::chrono::seconds(secondsUntilHandshake));

        secondsUntilHandshake *= 2;
    }

    if (!isReady) {
        LOG_ERROR("Client", "DoHandshake failed.");
    }
}
