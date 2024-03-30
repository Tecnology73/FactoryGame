#pragma once

#include <iostream>
#include <memory>
#include "../../Engine/Engine.h"
#include "../Packets/DestroyObjectPacket.h"

void DestroyObjectHandler(DestroyObjectPacket *packet, const SharedPtr<IClientConnection> &client) {
    auto object = ConnectionManager::GetNetworkObject(packet->networkId);
    if (!object) {
        LOG_ERROR("DestroyObjectHandler", "Object with (NetId: %llu) does not exist.", packet->networkId);
        return;
    }

    object->Destroy();
}