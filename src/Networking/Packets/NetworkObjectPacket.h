#pragma once

#include "../Packet.h"
#include "../NetworkObject.h"
#include "../../Engine/Common.h"

struct NetworkObjectPacket : Packet {
    uint64_t networkId = 0;

    using Packet::Packet;

    explicit NetworkObjectPacket(uint64_t networkId) : networkId(networkId) {}

    explicit NetworkObjectPacket(NetworkObject *networkObject) : networkId(networkObject->GetNetworkId()) {}

    void Serialize(ByteStream &stream) const override {
        Packet::Serialize(stream);
        stream.Write(networkId);
    }

    void Deserialize(ByteStream &stream) override {
        Packet::Deserialize(stream);
        stream.Read(networkId);
    }
};