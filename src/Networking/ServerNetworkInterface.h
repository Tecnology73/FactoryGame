#pragma once

#include <unordered_map>
#include <memory>
#include "./NetworkCore.h"
#include "../Server/ClientConnection.h"

class ServerNetworkInterface : public NetworkCore {
public:
    ServerNetworkInterface() : NetworkCore() {}

    int Init() override;

    void Start() override;

    /**
     * Events
     */
public:
    void OnClientConnected(SharedPtr<IClientConnection> client) override;

    void OnClientDisconnected(SharedPtr<IClientConnection> client) override;

    void OnObjectSpawned(const SharedPtr<NetworkObject> &object, const SharedPtr<IClientConnection> &owner);

    /**
     * Property Functions
     */
public:
    static ServerNetworkInterface *Get() { return (ServerNetworkInterface *) instance; }
};