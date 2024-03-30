#pragma once

#include <cstdint>
#include <memory>
#include "../Networking/IClientConnection.h"

/**
 * @brief Represents a remote RemoteClient connection on the LocalServer.
 * When playing single player, this is not used.
 * When hosting a server or a dedicated server, this will be used for each connected client.
 */
class ClientConnection : public IClientConnection {
public:
    explicit ClientConnection(IPAddress addr);

    ~ClientConnection() override;

    [[nodiscard]] IPAddress GetAddr() const { return addr; }

private:
    IPAddress addr;
};