#pragma once

#include <map>
#include <memory>
#include "Packet.h"
#include "../Util.h"
#include "../Engine/Common.h"

enum class NetworkRole : uint8_t {
    // We're a local client. (i.e. the player on this machine.)
    LocalClient,
    // We're a remote client. (i.e. a player on another machine, connected to this server.)
    RemoteClient,
    // We're a server. (This could be a self-hosted server, or a dedicated server.)
    LocalServer,
    // We're a remote server. (i.e. a server on another machine that we (the client) is connected to.)
    RemoteServer,
};

class NetworkObject;

class ConnectionManager;

class IClientConnection {
public:
    virtual ~IClientConnection() = default;

    /**
     * Object Management
     */
public:
    void AddObject(const SharedPtr<NetworkObject> &object);

    void RemoveObject(const SharedPtr<NetworkObject> &object);

    /**
     * Property Functions
     */
public:
    [[nodiscard]] uint32_t GetId() const;

    void SetId(uint32_t value);

    void SetRole(NetworkRole value);

    [[nodiscard]] bool IsLocalClient() const;

    [[nodiscard]] bool IsRemoteClient() const;

    [[nodiscard]] bool IsLocalServer() const;

    [[nodiscard]] bool IsRemoteServer() const;

    void SetPlayer(SharedPtr<NetworkObject> value);

    [[nodiscard]] SharedPtr<NetworkObject> GetPlayer() const;

protected:
    // 0 is invalid/pending connection.
    uint32_t networkId = 0;
    NetworkRole role = NetworkRole::LocalClient;
    SharedPtr<NetworkObject> player = nullptr;
    UnorderedMap<uint64_t, SharedPtr<NetworkObject>> ownedObjects;
};