#pragma once

#include "NetworkObjectPacket.h"

struct ClientDisconnectPacket : NetworkObjectPacket {
    using NetworkObjectPacket::NetworkObjectPacket;
};