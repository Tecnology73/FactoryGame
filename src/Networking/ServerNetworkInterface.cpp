#include <WinSock2.h>
#include "ServerNetworkInterface.h"
#include "../Game/Player.h"
#include "Packets/SpawnObjectPacket.h"
#include "Packets/ClientConnectedPacket.h"

int ServerNetworkInterface::Init() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return WSAGetLastError();
    }

    if ((networkSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        return WSAGetLastError();
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(NETWORK_PORT);
    if (bind(networkSocket, (SOCKADDR *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        return WSAGetLastError();
    }

    // Set blocking to non-blocking
    unsigned long nonBlocking = 1;
    int result = ioctlsocket(networkSocket, FIONBIO, &nonBlocking);
    if (result != NO_ERROR) {
        return result;
    }

    LOG_DEBUG("Server", "Server started on port %d", NETWORK_PORT);
    ConnectionManager::SetIsServer();
    networkId = 1;
    isReady = true;

    return 0;
}

void ServerNetworkInterface::Start() {}

void ServerNetworkInterface::OnClientConnected(SharedPtr<IClientConnection> client) {
    NetworkCore::SendPacket<ClientConnectedPacket>(client, client->GetId());

    // Synchronize all the currently spawned objects.
    {
        // FIXME: This is ***not*** thread safe!
        auto objects = ConnectionManager::GetNetworkObjects();
        LOG_DEBUG("Server", "Sending %d objects to client %d", objects.size(), client->GetId());
        for (const auto &item: objects) {
            // TODO: Implement packing of multiple objects into a single packet.
            NetworkCore::SendPacket<SpawnObjectPacket>(client, item.second.get());
        }
    }

    // Spawn the player.
    auto player = NetworkObject::Spawn<Player>("Player", {0, 0}, client);
    client->SetPlayer(player);
    client->SetRole(NetworkRole::RemoteClient);
    LOG_DEBUG("Server", "Client connected: %d | Player Owner ID: %d (Total: %d)", client->GetId(), player->GetOwnerId(), ConnectionManager::GetClients().size());
}

void ServerNetworkInterface::OnClientDisconnected(SharedPtr<IClientConnection> client) {
    LOG_DEBUG("Server", "Client disconnected: %d", client->GetId());
    ConnectionManager::RemoveClient(client);
}


void ServerNetworkInterface::OnObjectSpawned(const SharedPtr<NetworkObject> &object, const SharedPtr<IClientConnection> &owner) {
    LOG_DEBUG(
        "Server",
        "Object spawned: %llu | %d | %d | (%f, %f)",
        object->GetNetworkId(),
        object->GetNetworkTypeId(),
        object->GetOwnerId(),
        object->GetTransform()->position->x,
        object->GetTransform()->position->y
    );
    for (const auto &client: ConnectionManager::GetClients()) {
        // FIXME: Eat all the memory.
        NetworkCore::SendPacket<SpawnObjectPacket>(client.second, object.get());
    }
}
