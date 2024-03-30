#pragma once

#include <thread>
#include "./NetworkCore.h"

#define HANDSHAKE_TIMEOUT 2
#define HANDSHAKE_MAX_ATTEMPTS 5

class ClientNetworkInterface : public NetworkCore {
public:
    explicit ClientNetworkInterface() : NetworkCore() {}

    ~ClientNetworkInterface() override;

    int Init() override;

    void Start() override;

    void Shutdown() override;

    /**
     * Events
     */
public:
    void OnHandshakeCompleted(uint32_t networkId) override;

private:
    void DoHandshake();

private:
    std::thread handshakeThread;
};