#include "NetworkObject.h"
#include "NetworkCore.h"
#include "NetworkObjectFactory.h"
#include "Packets/PlayerPositionUpdatePacket.h"
#include "Packets/DestroyObjectPacket.h"

void NetworkObject::Init() {
    Object::Init();

    // Track the NetworkObject.
    ConnectionManager::AddNetworkObject(std::dynamic_pointer_cast<NetworkObject>(shared_from_this()));
}

void NetworkObject::OnDestroy() {
    Object::OnDestroy();

    if (ConnectionManager::IsServer()) {
        // Destroy the object on all the clients.
        NetworkCore::BroadcastExcept<DestroyObjectPacket>(owner, networkId);
        // Remove the object from the owner's list of objects.
        owner->RemoveObject(std::dynamic_pointer_cast<NetworkObject>(shared_from_this()));
    }

    // Stop tracking the NetworkObject.
    ConnectionManager::RemoveNetworkObject(networkId);
}

void NetworkObject::Update(float deltaTime) {
    Object::Update(deltaTime);

    if (!IsOwner()) {
        return;
    }

    nextNetworkUpdate -= deltaTime;
    if (nextNetworkUpdate <= 0.f) {
        nextNetworkUpdate = networkUpdateFrequency;
        NetworkUpdate();
    }
}

void NetworkObject::NetworkUpdate() {
    if (!IsOwner()) {
        return;
    }

    auto transform = GetTransform();
    if (!transform->IsDirty()) return;

    SendPacket<PlayerPositionUpdatePacket>(this);
}

uint16_t NetworkObject::GetNetworkTypeId() const {
    return NetworkObjectFactory::GetId(typeid(*this));
}

uint64_t NetworkObject::GetNetworkId() const {
    return networkId;
}

void NetworkObject::SetNetworkId(uint64_t id) {
    networkId = id;
}

uint32_t NetworkObject::GetOwnerId() const {
    return ownerId;
}

void NetworkObject::SetOwner(uint32_t value) {
    ownerId = value;

    if (ConnectionManager::IsServer()) {
        owner = ConnectionManager::GetClient(ownerId);
        owner->AddObject(std::dynamic_pointer_cast<NetworkObject>(shared_from_this()));
    }
}

bool NetworkObject::IsOwner() const {
    return NetworkCore::GetNetworkId() == ownerId;
}
