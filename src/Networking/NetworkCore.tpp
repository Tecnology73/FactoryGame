#pragma once

#include "ConnectionManager.h"
#include "IClientConnection.h"

template<typename T, typename... Args>
void NetworkCore::SendPacket(SharedPtr<IClientConnection> client, Args &&... args) {
    static_assert(std::is_base_of<Packet, T>::value, "T must derive from Packet.");
    instance->SendPacket(client, MakeUnique<T>(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
void NetworkCore::Broadcast(Args &&... args) {
    static_assert(std::is_base_of<Packet, T>::value, "T must derive from Packet.");
    for (auto &client: ConnectionManager::GetClients()) {
        instance->SendPacket<T>(client.second, std::forward<Args>(args)...);
    }
}

template<typename T, typename... Args>
void NetworkCore::BroadcastExcept(const SharedPtr<IClientConnection> &excludedClient, Args &&... args) {
    static_assert(std::is_base_of<Packet, T>::value, "T must derive from Packet.");
    for (auto &client: ConnectionManager::GetClients()) {
        if (client.first != excludedClient->GetId()) {
            instance->SendPacket<T>(client.second, std::forward<Args>(args)...);
        }
    }
}

template<typename T, typename... Args>
bool NetworkCore::SendPacketImmediate(SharedPtr<IClientConnection> client, Args &&... args) {
    static_assert(std::is_base_of<Packet, T>::value, "T must derive from Packet.");

    return instance->SendPacketImmediate(client, MakeUnique<T>(std::forward<Args>(args)...));
}
