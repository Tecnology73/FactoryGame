#pragma once

#include "NetworkObjectPacket.h"

struct DestroyObjectPacket : NetworkObjectPacket {
    using NetworkObjectPacket::NetworkObjectPacket;
};