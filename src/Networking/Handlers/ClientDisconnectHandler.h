#pragma once

#include <iostream>
#include <memory>
#include "../../Engine/Engine.h"
#include "../Packets/ClientDisconnectPacket.h"

void ClientDisconnectHandler(ClientDisconnectPacket *packet, const SharedPtr<IClientConnection> &client) {
    if (!ConnectionManager::IsServer()) {
        LOG_ERROR("ClientDisconnectPacket.h", "Tried to process on non-server.");
        return;
    }

    NetworkCore::Get()->OnClientDisconnected(client);
}