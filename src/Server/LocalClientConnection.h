#pragma once

#include "../Networking/IClientConnection.h"

/**
 * @brief Represents the local RemoteClient.
 * When playing single player or hosting a server, this will be the local client.
 * On a dedicated server, this will not be used (as there is no local player.)
 */
class LocalClientConnection : public IClientConnection {
public:
};