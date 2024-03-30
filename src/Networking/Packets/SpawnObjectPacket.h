#pragma once

#include "NetworkObjectPacket.h"

struct SpawnObjectPacket : NetworkObjectPacket {
    uint16_t typeId;
    uint32_t ownerId;
    Vec2 position;

    using NetworkObjectPacket::NetworkObjectPacket;

    explicit SpawnObjectPacket(NetworkObject *obj) :
        NetworkObjectPacket(obj->GetNetworkId()),
        typeId(obj->GetNetworkTypeId()),
        ownerId(obj->GetOwnerId()),
        position(obj->GetTransform()->position) {}

    void Serialize(ByteStream &stream) const override {
        NetworkObjectPacket::Serialize(stream);
        stream.Write(typeId);
        stream.Write(ownerId);
        position.Serialize(stream);
        LOG_DEBUG("SpawnObjectPacket", "Serialized spawn object packet | NetID: %llu | Position: (%f, %f)", networkId, position.x, position.y);
    }

    void Deserialize(ByteStream &stream) override {
        NetworkObjectPacket::Deserialize(stream);
        stream.Read(typeId);
        stream.Read(ownerId);
        position.Deserialize(stream);
        LOG_DEBUG("SpawnObjectPacket", "Deserialized spawn object packet | NetID: %llu | Position: (%f, %f)", networkId, position.x, position.y);
    }
};