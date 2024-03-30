#pragma once

#include <iostream>
#include <memory>
#include "../../Engine/Engine.h"
#include "../NetworkObjectFactory.h"
#include "../Packets/SpawnObjectPacket.h"

void SpawnObjectHandler(SpawnObjectPacket *packet, const SharedPtr<IClientConnection> &client) {
    if (!ConnectionManager::IsClient()) {
        LOG_ERROR("SpawnObjectHandler", "Tried to process on non-client.");
        return;
    }

    // LOG_DEBUG("SpawnObjectHandler", "Spawned object: \n\tNetworkID: %llu\n\tOwner ID: %hu\n\tPosition: (%f, %f)", p->networkId, p->ownerId, p->position.x, p->position.y);
    NetworkObjectFactory::Create(packet);
}