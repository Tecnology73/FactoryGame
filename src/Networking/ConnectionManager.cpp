#include "ConnectionManager.h"
#include "IClientConnection.h"
#include "NetworkObject.h"

ConnectionManager *ConnectionManager::instance = nullptr;

ConnectionManager::ConnectionManager() {
    instance = this;
}

const Map<uint32_t, SharedPtr<IClientConnection>> &ConnectionManager::GetClients() {
    return instance->clients;
}

SharedPtr<IClientConnection> ConnectionManager::GetClient(uint32_t networkId) {
    auto it = instance->clients.find(networkId);
    if (it == instance->clients.end()) {
        return nullptr;
    }

    return it->second;
}

SharedPtr<IClientConnection> ConnectionManager::GetOrCreateClient(uint32_t senderId, const IPAddress &addr) {
    if (IsClient()) {
        // If we're a client, the addr will always be the Servers.
        // So we'll look up clients based on the senderId instead.
        auto it = instance->clients.find(senderId);
        if (it == instance->clients.end()) {
            return CreateClient(addr, senderId);
        }

        return it->second;
    }

    // If we're a server, the addr will always be the Clients.
    auto it = instance->clientAddrMap.find(addr);
    if (it == instance->clientAddrMap.end()) {
        return CreateClient(addr, senderId);
    }

    return GetClient(it->second);
}

bool ConnectionManager::IsClient() {
    return instance->role == NetworkRole::LocalClient;
}

bool ConnectionManager::IsServer() {
    return instance->role == NetworkRole::LocalServer;
}

void ConnectionManager::SetIsClient() {
    instance->role = NetworkRole::LocalClient;
}

void ConnectionManager::SetIsServer() {
    instance->role = NetworkRole::LocalServer;
}

SharedPtr<ClientConnection> ConnectionManager::GetServer() {
    return instance->server;
}

void ConnectionManager::SetServer(const IPAddress &addr) {
    instance->server = MakeUnique<ClientConnection>(addr);
    instance->server->SetId(1); // A server always has an ID of 1.
    instance->server->SetRole(NetworkRole::RemoteServer);
}

SharedPtr<IClientConnection> ConnectionManager::CreateClient(const IPAddress &addr, uint32_t senderId) {
    // 0 is reserved for unknown client state (before they perform a handshake).
    // 1 is reserved for the Server.
    static uint32_t nextClientId = 2;
    if (senderId > 1 && nextClientId <= senderId) {
        nextClientId = senderId + 1;
    } else {
        senderId = nextClientId++;
    }

    auto client = MakeShared<ClientConnection>(addr);
    client->SetId(senderId);

    instance->clients.emplace(senderId, client);
    instance->clientAddrMap.emplace(addr, senderId);

    return client;
}

void ConnectionManager::RemoveClient(const SharedPtr<IClientConnection> &client) {
    auto remoteClient = std::dynamic_pointer_cast<ClientConnection>(client);
    if (remoteClient == nullptr) {
        LOG_WARN("ConnectionManager", "Trying to remove a client that isn't a ClientConnection!");
        return;
    }

    if (!remoteClient->IsRemoteClient()) {
        LOG_WARN("ConnectionManager", "Trying to remove a client that isn't a remote client!");
        return;
    }

    remoteClient->GetPlayer()->Destroy();
    remoteClient->SetPlayer(nullptr);

    instance->clientAddrMap.erase(remoteClient->GetAddr());
    instance->clients.erase(remoteClient->GetId());
}

const UnorderedMap<uint64_t, SharedPtr<NetworkObject>> &ConnectionManager::GetNetworkObjects() {
    return instance->networkObjects;
}

SharedPtr<NetworkObject> ConnectionManager::GetNetworkObject(uint64_t networkId) {
    auto it = instance->networkObjects.find(networkId);
    if (it == instance->networkObjects.end()) {
        return nullptr;
    }

    return it->second;
}

void ConnectionManager::AddNetworkObject(const SharedPtr<NetworkObject> &object) {
    std::lock_guard<std::mutex> lock(instance->networkObjectsMutex);
    if (instance->networkObjects.contains(object->GetNetworkId())) {
        LOG_WARN(
            "ConnectionManager",
            "Trying to add a network object with ID that is already assigned! %s (ID: %llu | NetId: %llu) is assigned to %s (ID: %llu | NetId: %llu)",
            object->GetName().c_str(),
            object->GetId(),
            object->GetNetworkId(),
            GetNetworkObject(object->GetNetworkId())->GetName().c_str(),
            GetNetworkObject(object->GetNetworkId())->GetId(),
            GetNetworkObject(object->GetNetworkId())->GetNetworkId()
        );
        return;
    }

    instance->networkObjects.emplace(object->GetNetworkId(), object);
}

void ConnectionManager::RemoveNetworkObject(uint64_t networkId) {
    if (!instance->networkObjects.contains(networkId)) {
        LOG_WARN("ConnectionManager", "Trying to remove a network object that doesn't exist! (NetId: %llu)", networkId);
        return;
    }

    std::lock_guard<std::mutex> lock(instance->networkObjectsMutex);
    instance->networkObjects.erase(networkId);
}
