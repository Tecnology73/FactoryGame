#include "NetworkObjectFactory.h"
#include "../Engine/Object.h"
#include "Packets/SpawnObjectPacket.h"
// Spawn-able Objects
#include "NetworkObject.h"
#include "../Game/Player.h"

NetworkObjectFactory *NetworkObjectFactory::instance = nullptr;

NetworkObjectFactory::NetworkObjectFactory() {
    instance = this;
}

int NetworkObjectFactory::Init() {
    static uint16_t typeId = 1; // 0 is used to indicate an invalid type.
#define REGISTER_NETWORK_OBJECT(type) \
    factories.emplace(typeId, [](SpawnObjectPacket* packet) { \
        auto obj = Object::Create<type>(packet->position); \
        obj->SetNetworkId(packet->networkId); \
        obj->SetOwner(packet->ownerId); \
        /* Register the object with the ConnectionManager */ \
        ConnectionManager::AddNetworkObject(obj); \
        return obj; \
    }); \
    typeToId.emplace(std::type_index(typeid(type)), typeId++)

    REGISTER_NETWORK_OBJECT(NetworkObject);
    REGISTER_NETWORK_OBJECT(Player);

    return 0;
}

uint16_t NetworkObjectFactory::GetId(const std::type_info &typeInfo) {
    auto it = instance->typeToId.find(std::type_index(typeInfo));
    if (it == instance->typeToId.end()) {
        LOG_ERROR("NetworkObjectFactory", "Tried to get ID of unregistered network object type.");
        return 0;
    }

    return it->second;
}

SharedPtr<NetworkObject> NetworkObjectFactory::Create(SpawnObjectPacket *packet) {
    auto factory = instance->factories.find(packet->typeId);
    if (factory == instance->factories.end()) {
        LOG_ERROR("NetworkObjectFactory", "Tried to create unregistered network object type.");
        return nullptr;
    }

    return factory->second(packet);
}