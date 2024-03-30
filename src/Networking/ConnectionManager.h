#pragma once

#include <map>
#include <memory>
#include <mutex>
#include "../Util.h"
#include "../Engine/ISystem.h"
#include "../Server/ClientConnection.h"

class ConnectionManager : public ISystem {
public:
    ConnectionManager();

    static std::string GetName() { return "Connection Manager"; }

    int Init() override { return 0; }

    /**
     * Client Management
     */
public:
    static const Map<uint32_t, SharedPtr<IClientConnection>> &GetClients();

    static SharedPtr<IClientConnection> GetClient(uint32_t networkId);

    static SharedPtr<IClientConnection> GetOrCreateClient(uint32_t senderId, const IPAddress &addr);

    static void RemoveClient(const SharedPtr<IClientConnection> &client);

    /**
     * Network Object Management
     */
public:
    static const UnorderedMap<uint64_t, SharedPtr<NetworkObject>> &GetNetworkObjects();

    static SharedPtr<NetworkObject> GetNetworkObject(uint64_t networkId);

    static void AddNetworkObject(const SharedPtr<NetworkObject> &object);

    static void RemoveNetworkObject(uint64_t networkId);

    /**
     * Property Functions
     */
public:
    static bool IsClient();

    static bool IsServer();

    static void SetIsClient();

    static void SetIsServer();

    static SharedPtr<ClientConnection> GetServer();

    static void SetServer(const IPAddress &addr);

private:
    static SharedPtr<IClientConnection> CreateClient(const IPAddress &addr, uint32_t senderId);

private:
    static ConnectionManager *instance;

    // The role for this machine.
    // This can only be either LocalClient or LocalServer.
    NetworkRole role = NetworkRole::LocalClient;

    // Used by clients to store the server connection.
    // Just for easier access, so we don't have to look it up in the map.
    SharedPtr<ClientConnection> server;

    Map<uint32_t, SharedPtr<IClientConnection>> clients;
    Map<IPAddress, uint32_t> clientAddrMap;

    // networkId -> NetworkObjects.
    std::mutex networkObjectsMutex;
    UnorderedMap<uint64_t, SharedPtr<NetworkObject>> networkObjects;
};