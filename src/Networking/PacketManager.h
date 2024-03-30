#pragma once

#include <functional>
#include <typeindex>
#include "Packet.h"
#include "../Engine/ISystem.h"
#include "IClientConnection.h"

class PacketManager : public ISystem {
public:
    template<typename T>
    using HandlerFunc = std::function<void(UniquePtr<T>, const SharedPtr<IClientConnection> &)>;

    PacketManager();

    [[nodiscard]] static std::string GetName() { return "PacketManager"; }

    int Init() override;

    static uint16_t GetType(Packet *packet) {
        auto it = instance->packetTypes.find(typeid(*packet));
        if (it == instance->packetTypes.end()) {
            return 0; // Unknown
        }

        return it->second;
    }

    static bool Dispatch(const SharedPtr<IClientConnection> &client, UniquePtr<Packet> packet) {
        auto it = instance->handlers.find(packet->type);
        if (it == instance->handlers.end()) {
            return false;
        }

        it->second(std::move(packet), client);
        return true;
    }

    static std::unique_ptr<Packet> Deserialize(uint16_t type, ByteStream &stream) {
        auto it = instance->factories.find(type);
        if (it == instance->factories.end()) {
            return nullptr;
        }

        return it->second(type, stream);
    }

private:
    template<typename T>
    void Register(const std::function<void(T *, const SharedPtr<IClientConnection> &)> &handlerFunc) {
        static_assert(std::is_base_of<Packet, T>::value, "T must be a Packet");

        factories.emplace(packetId, [](uint16_t type, ByteStream &stream) {
            auto packet = MakeUnique<T>();
            packet->type = type;
            packet->Deserialize(stream);
            return packet;
        });
        handlers.emplace(packetId, [handlerFunc](UniquePtr<Packet> packet, const SharedPtr<IClientConnection> &client) {
            handlerFunc(dynamic_cast<T *>(packet.get()), client);
        });
        packetTypes.emplace(typeid(T), packetId);

        LOG_DEBUG("PacketManager", "Registered packet type %d (%s)", packetId, typeid(T).name());

        packetId++;
    }

private:
    static PacketManager *instance;
    uint16_t packetId = 1;

    UnorderedMap<uint16_t, std::function<UniquePtr<Packet>(uint16_t, ByteStream &)>> factories;
    UnorderedMap<uint16_t, HandlerFunc<Packet>> handlers;
    UnorderedMap<std::type_index, uint16_t> packetTypes;
};
