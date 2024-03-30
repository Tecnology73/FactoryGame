#include "ClientConnection.h"
#include <utility>
#include "../Networking/NetworkObject.h"

ClientConnection::ClientConnection(IPAddress addr) :
    IClientConnection(),
    addr(std::move(addr)) {
}

ClientConnection::~ClientConnection() {
    for (const auto &item: ownedObjects) {
        item.second->Destroy();
    }
}
