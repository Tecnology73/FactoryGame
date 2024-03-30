#pragma once

#include "../Engine/Object.h"
#include "NetworkCore.h"
#include "ConnectionManager.h"

class IClientConnection;

struct Packet;

class NetworkObject : public Object {
public:
    using Object::Object;
    
    void Init() override;

    void OnDestroy() override;

    void Update(float deltaTime) override;

    virtual void NetworkUpdate();

    /**
     * Static Functions
     */
public:
    template<typename T>
    static SharedPtr<T> Spawn(const std::string &name, Vec2 position, const SharedPtr<IClientConnection> &owner);

    /**
     * Property Functions
     */
public:
    uint16_t GetNetworkTypeId() const;

    uint64_t GetNetworkId() const;

    void SetNetworkId(uint64_t id);

    uint32_t GetOwnerId() const;

    void SetOwner(uint32_t value);

    bool IsOwner() const;

protected:
    template<typename T, typename... Args>
    void SendPacket(Args &&... args);

private:
    // This is the ID of the object on the server (from the server's EntityManager).
    uint64_t networkId = 0;
    // This is the ID of the client that owns this object.
    uint32_t ownerId = 0;
    // This is used on the Server just as a cache to avoid having to look up the owner in the ConnectionManager.
    SharedPtr<IClientConnection> owner = nullptr;

    // How often to send network updates
    float networkUpdateFrequency = 1.f / 20.f;
    // Time until next network update
    float nextNetworkUpdate = networkUpdateFrequency;
};

#include "NetworkObject.tpp"