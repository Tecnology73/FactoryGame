#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include "../Engine/ISystem.h"
#include "../Engine/Common.h"

class NetworkObject;

struct SpawnObjectPacket;

class NetworkObjectFactory : public ISystem {
public:
    NetworkObjectFactory();

    [[nodiscard]] static std::string GetName() { return "NetworkObjectFactory"; }

    [[nodiscard]] int Init() override;

    [[nodiscard]] static uint16_t GetId(const std::type_info &typeInfo);

    template<typename T>
    [[nodiscard]] static uint16_t GetId() {
        return GetId(typeid(T));
    }

    static std::shared_ptr<NetworkObject> Create(SpawnObjectPacket *packet);

private:
    static NetworkObjectFactory *instance;

    // Key = Network Type ID
    std::unordered_map<uint16_t, std::function<std::shared_ptr<NetworkObject>(SpawnObjectPacket *)>> factories;
    std::unordered_map<std::type_index, uint16_t> typeToId;
};