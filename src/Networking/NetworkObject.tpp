#pragma once

#include "../Engine/EntityManager.h"
#include "ConnectionManager.h"
#include "ServerNetworkInterface.h"

template<typename T>
SharedPtr<T> NetworkObject::Spawn(const std::string &name, Vec2 position, const SharedPtr<IClientConnection> &owner) {
    static_assert(std::is_base_of<NetworkObject, T>::value, "T must inherit from NetworkObject");

    if (!ConnectionManager::IsServer()) {
        LOG_ERROR("NetworkObject", "Tried to spawn object on non-server.");
        return nullptr;
    }

    // Can't use `NetworkObjectFactory::Create` because we'd need to pass in the networkId which we don't know yet.
    auto object = Object::Create<T>(position);
    object->SetNetworkId(object->GetId());
    object->SetName(name);
    object->SetOwner(owner->GetId());

    // Track the NetworkObject and replicate to clients.
    ConnectionManager::AddNetworkObject(object);
    ServerNetworkInterface::Get()->OnObjectSpawned(object, owner);

    return object;
}

template<typename T, typename... Args>
void NetworkObject::SendPacket(Args &&... args) {
    NetworkCore::SendPacket<T>(ConnectionManager::GetServer(), std::forward<Args>(args)...);
}