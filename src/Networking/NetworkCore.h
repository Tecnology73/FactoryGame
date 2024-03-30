#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <functional>
#include <utility>
#include "Packet.h"
#include "../Util.h"
#include "IClientConnection.h"
#include "../Engine/ISystem.h"

#define NETWORK_IP "127.0.0.1"
#define NETWORK_PORT 42069
#define NETWORK_MSG_BUFFER_SIZE 1024
#define MAX_INCOMING_PACKETS_PER_FRAME 10
#define MAX_PROCESSING_PACKETS_PER_FRAME 10
#define MAX_OUTGOING_PACKETS_PER_FRAME 10

struct PendingPacket {
    PendingPacket(SharedPtr<IClientConnection> client, UniquePtr<Packet> packet) :
        client(std::move(client)),
        packet(std::move(packet)) {}

    SharedPtr<IClientConnection> client;
    UniquePtr<Packet> packet;
};

class NetworkCore : public ISystem {
public:
    NetworkCore();

    [[nodiscard]] static std::string GetName() { return "NetworkCore"; }

    void PreUpdate(float deltaTime) override;

    void PostUpdate(float deltaTime) override;

    /**
     * Packets
     */
public:
    template<typename T, typename... Args>
    static void SendPacket(SharedPtr<IClientConnection> client, Args &&... args);

    void SendPacket(const SharedPtr<IClientConnection> &client, std::unique_ptr<Packet> packet);

    template<typename T, typename... Args>
    static void Broadcast(Args &&... args);

    template<typename T, typename... Args>
    static void BroadcastExcept(const SharedPtr<IClientConnection> &excludedClient, Args &&... args);

    template<typename T, typename... Args>
    static bool SendPacketImmediate(SharedPtr<IClientConnection> client, Args &&... args);

    bool SendPacketImmediate(const SharedPtr<IClientConnection> &client, std::unique_ptr<Packet> packet);

    /**
     * Property Functions
     */
public:
    [[nodiscard]] bool IsReady() const { return isReady; }

    [[nodiscard]] static uint32_t GetNetworkId() { return instance->networkId; }

    static NetworkCore *Get() { return instance; }

    /**
    * Events
    */
public:
    virtual void OnClientConnected(SharedPtr<IClientConnection> client) {}

    virtual void OnClientDisconnected(SharedPtr<IClientConnection> client) {}

    virtual void OnHandshakeCompleted(uint32_t netId) {}

    /**
     *
     */
private:
    void ReceiveIncomingPackets();

    void ProcessIncomingPackets();

    void SendOutgoingPackets();

protected:
    static NetworkCore *instance;

    // For the server, this is always 1.
    // For the client, the server sends this (via ClientConnectedPacket) after the handshake is complete.
    // 0 = uninitialized.
    uint32_t networkId = 0;

    uint32_t networkSocket;
    IPAddress networkAddress;
    bool isReady = false;

    /**
     * Queue for incoming packets.
     * These could be received over the network or come directly from the local client if running a self-hosted server.
     */
    std::queue<std::unique_ptr<PendingPacket>> incomingPackets;
    std::mutex incomingPacketsMutex;

    /**
     * Queue for outgoing packets.
     * These are queued during the game's Update loop and sent at the end of the frame.
     */
    std::queue<std::unique_ptr<PendingPacket>> outgoingPackets;
    std::mutex outgoingPacketsMutex;
};

#include "NetworkCore.tpp"