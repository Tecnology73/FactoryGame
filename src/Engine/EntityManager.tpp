#pragma once

/*
template<typename T>
SharedPtr<T> EntityManager::Create(Vec2 position, uint64_t id) {
    static_assert(std::is_base_of<Object, T>::value, "T must inherit from Object");

    static uint64_t nextId = 1;
    if (id > 0 && nextId <= id) {
        nextId = id + 1;
    } else {
        id = nextId++;
    }

    auto object = std::make_shared<T>(id, position);
    instance->objects.emplace(id, object);

    return object;
}
*/

template<typename T>
void EntityManager::Register(SharedPtr<T> object) {
    if (instance->objects.contains(object->GetId())) {
        LOG_ERROR("EntityManager", "Object with id %llu already exists.", object->GetId());
        return;
    }

    instance->objects.emplace(object->GetId(), object);
    instance->objectNameToId[object->GetName()].push_back(object->GetId());
}

template<typename T>
SharedPtr<T> EntityManager::Get(uint64_t id) {
    static_assert(std::is_base_of<Object, T>::value, "T must inherit from Object");

    auto it = instance->objects.find(id);
    if (it == instance->objects.end()) {
        return nullptr;
    }

    return std::dynamic_pointer_cast<T>(it->second);
}

template<typename T>
void EntityManager::Find(const std::string &name, Vector<SharedPtr<T>> &matchingObjects) {
    static_assert(std::is_base_of<Object, T>::value, "T must inherit from Object");

    auto it = instance->objectNameToId.find(name);
    if (it == instance->objectNameToId.end()) {
        return;
    }

    for (const auto &id: it->second) {
        auto object = Get<T>(id);
        if (object) {
            matchingObjects.push_back(object);
        }
    }
}
