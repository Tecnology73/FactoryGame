#pragma once

#include <iostream>
#include <memory>
#include "../../Engine/Engine.h"
#include "../NetworkObject.h"
#include "../Packets/PlayerPositionUpdatePacket.h"

void PlayerPositionUpdateHandler(PlayerPositionUpdatePacket *packet, const SharedPtr<IClientConnection> &client) {
    if (ConnectionManager::IsServer()) {
        auto transform = client->GetPlayer()->GetTransform();
        transform->position = packet->position;
        transform->size = packet->size;
        transform->rotation = packet->rotation;

        NetworkCore::BroadcastExcept<PlayerPositionUpdatePacket>(client, client->GetPlayer().get());
        return;
    }

    auto object = ConnectionManager::GetNetworkObject(packet->networkId);
    if (!object) {
        LOG_ERROR("PlayerPositionUpdateHandler", "Failed to find object with network ID %llu.", packet->networkId);
        return;
    }

    auto transform = object->GetTransform();
    transform->position = packet->position;
    transform->size = packet->size;
    transform->rotation = packet->rotation;
}