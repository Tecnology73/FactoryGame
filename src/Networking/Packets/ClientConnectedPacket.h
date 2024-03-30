#pragma once

#include "NetworkObjectPacket.h"

struct ClientConnectedPacket : public NetworkObjectPacket {
    using NetworkObjectPacket::NetworkObjectPacket;
};