#pragma once

#include <string>
#include <memory>
#include "ISystem.h"
#include "Common.h"
#include "../Util.h"

class Object;

class EntityManager : public ISystem {
public:
    EntityManager();

    [[nodiscard]] static std::string GetName() { return "EntityManager"; }

    [[nodiscard]] int Init() override { return 0; }

    [[nodiscard]] static const Map<uint64_t, SharedPtr<Object>> &GetObjects();

    static uint64_t GetNextId() {
        static uint64_t nextId = 1; // 0 is reserved for uninitialized Objects.
        return nextId++;
    }
    
    template<typename T>
    static void Register(SharedPtr<T> object);

    template<typename T>
    static std::shared_ptr<T> Get(uint64_t id);

    template<typename T>
    static void Find(const std::string &name, Vector<SharedPtr<T>> &matchingObjects);

    static void Destroy(uint64_t id);

    static EntityManager *Get() { return instance; }

private:
    static EntityManager *instance;

    Map<uint64_t, SharedPtr<Object>> objects;
    UnorderedMap<std::string, Vector<uint64_t>> objectNameToId;
};

#include "EntityManager.tpp"