#include "PacketManager.h"
// Packets
#include "./Packets/ClientConnectPacket.h"
#include "./Packets/ClientConnectedPacket.h"
#include "./Packets/ClientDisconnectPacket.h"
#include "./Packets/SpawnObjectPacket.h"
#include "./Packets/DestroyObjectPacket.h"
#include "./Packets/PlayerPositionUpdatePacket.h"
// Handlers
#include "./Handlers/ClientConnectHandler.h"
#include "./Handlers/ClientConnectedHandler.h"
#include "./Handlers/ClientDisconnectHandler.h"
#include "./Handlers/SpawnObjectHandler.h"
#include "./Handlers/DestroyObjectHandler.h"
#include "./Handlers/PlayerPositionUpdateHandler.h"

PacketManager *PacketManager::instance = nullptr;

PacketManager::PacketManager() {
    instance = this;
}

int PacketManager::Init() {
    Register<ClientConnectPacket>(ClientConnectHandler);
    Register<ClientConnectedPacket>(ClientConnectedHandler);
    Register<ClientDisconnectPacket>(ClientDisconnectHandler);
    Register<SpawnObjectPacket>(SpawnObjectHandler);
    Register<DestroyObjectPacket>(DestroyObjectHandler);
    Register<PlayerPositionUpdatePacket>(PlayerPositionUpdateHandler);

    return 0;
}
