#pragma once

#include <iostream>
#include <memory>
#include "../../Engine/Engine.h"
#include "../Packets/ClientConnectedPacket.h"

void ClientConnectedHandler(ClientConnectedPacket *packet, const SharedPtr<IClientConnection> &client) {
    if (!ConnectionManager::IsClient()) {
        LOG_ERROR("ClientConnectedPacket", "Tried to process on non-client.");
        return;
    }

    // LOG_DEBUG("ClientConnectedPacket", "%hu", p->id);
    NetworkCore::Get()->OnHandshakeCompleted(static_cast<uint32_t>(packet->networkId));
}