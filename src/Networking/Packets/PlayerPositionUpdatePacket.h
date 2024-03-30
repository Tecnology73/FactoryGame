#pragma once

#include "NetworkObjectPacket.h"

struct PlayerPositionUpdatePacket : NetworkObjectPacket {
    // We _could_ serialize the Transform component, but I don't think that's a good idea.
    Vec2 position;
    Vec2 size;
    float rotation;

    using NetworkObjectPacket::NetworkObjectPacket;

    explicit PlayerPositionUpdatePacket(NetworkObject *player) :
        NetworkObjectPacket(player) {
        auto transform = player->GetTransform();
        position = transform->position;
        size = transform->size;
        rotation = transform->rotation;
    }

    void Serialize(ByteStream &stream) const override {
        NetworkObjectPacket::Serialize(stream);
        position.Serialize(stream);
        size.Serialize(stream);
        stream.Write(rotation);
    }

    void Deserialize(ByteStream &stream) override {
        NetworkObjectPacket::Deserialize(stream);
        position.Deserialize(stream);
        size.Deserialize(stream);
        stream.Read(rotation);
    }
};