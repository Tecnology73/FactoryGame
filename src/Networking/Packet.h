#pragma once

#include <memory>
#include "ByteStream.h"

static const size_t MIN_PACKET_SIZE = sizeof(uint16_t) + sizeof(uint32_t);

struct Packet {
    uint16_t type = 0;
    uint32_t sender = 0;

    Packet() = default;

    virtual void Serialize(ByteStream &stream) const {
        stream.Write(type);
        stream.Write(sender);
    }

    virtual void Deserialize(ByteStream &stream) {
        // type is already read before calling this method.
        stream.Read(sender);
    }
};