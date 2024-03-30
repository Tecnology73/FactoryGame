#include "EntityManager.h"

EntityManager *EntityManager::instance = nullptr;

EntityManager::EntityManager() {
    instance = this;
}

const Map<uint64_t, SharedPtr<Object>> &EntityManager::GetObjects() {
    return instance->objects;
}

void EntityManager::Destroy(uint64_t id) {
    auto it = instance->objects.find(id);
    if (it == instance->objects.end()) {
        return;
    }

    instance->objects.erase(it);
}
