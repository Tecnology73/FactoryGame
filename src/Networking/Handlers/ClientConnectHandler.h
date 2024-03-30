#pragma once

#include <iostream>
#include <memory>
#include "../../Engine/Engine.h"
#include "../Packets/ClientConnectPacket.h"

void ClientConnectHandler(ClientConnectPacket *packet, const SharedPtr<IClientConnection> &client) {
    if (!ConnectionManager::IsServer()) {
        LOG_ERROR("ClientConnectPacket", "Tried to process on non-server.");
        return;
    }

    // LOG_DEBUG("ClientConnectPacket", "RemoteClient connected: %hu", client->GetId());
    NetworkCore::Get()->OnClientConnected(client);
}