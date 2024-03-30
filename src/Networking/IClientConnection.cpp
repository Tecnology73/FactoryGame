#include "IClientConnection.h"

#include <utility>
#include "ConnectionManager.h"
#include "../Engine/EntityManager.h"
#include "NetworkObject.h"

uint32_t IClientConnection::GetId() const {
    return networkId;
}

void IClientConnection::SetId(uint32_t value) {
    if (networkId != 0) {
        LOG_ERROR("IClientConnection", "Trying to set value on client connection that already has an value.");
        return;
    }

    networkId = value;
}

void IClientConnection::SetRole(NetworkRole value) {
    role = value;
}

bool IClientConnection::IsLocalClient() const {
    return role == NetworkRole::LocalClient;
}

bool IClientConnection::IsRemoteClient() const {
    return role == NetworkRole::RemoteClient;
}

bool IClientConnection::IsLocalServer() const {
    return role == NetworkRole::LocalServer;
}

bool IClientConnection::IsRemoteServer() const {
    return role == NetworkRole::RemoteServer;
}

void IClientConnection::SetPlayer(SharedPtr<NetworkObject> value) {
    if (!ConnectionManager::IsServer()) {
        LOG_ERROR("IClientConnection", "Trying to set player on non-server.");
        return;
    }
    
    player = value;
}

SharedPtr<NetworkObject> IClientConnection::GetPlayer() const {
    return player;
}

void IClientConnection::AddObject(const SharedPtr<NetworkObject> &object) {
    if (!ConnectionManager::IsServer()) {
        LOG_ERROR("IClientConnection", "Trying to add object on non-server.");
        return;
    }

    ownedObjects.emplace(object->GetNetworkId(), object);
}

void IClientConnection::RemoveObject(const SharedPtr<NetworkObject> &object) {
    auto it = ownedObjects.find(object->GetNetworkId());
    if (it == ownedObjects.end()) {
        LOG_ERROR("IClientConnection", "Trying to remove object that isn't owned by this client.");
        return;
    }

    ownedObjects.erase(it);
}
