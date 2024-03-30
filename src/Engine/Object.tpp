#pragma once

#include "EntityManager.h"

template<typename T>
SharedPtr<T> Object::Create(const Vec2 &position) {
    auto obj = MakeShared<T>(EntityManager::GetNextId(), position);
    EntityManager::Register(obj);

    return obj;
}

template<typename T>
T *Object::AddComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    auto component = MakeUnique<T>(this);
    auto ptr = component.get();
    components[std::type_index(typeid(T))] = std::move(component);

    return ptr;
}

template<typename T>
T *Object::GetComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    auto t = std::type_index(typeid(T));
    auto it = components.find(t);
    if (it == components.end()) {
        LOG_WARN("Object", "Component %s does not exist.", typeid(T).name());
        return nullptr;
    }

    return dynamic_cast<T *>(it->second.get());
}

template<typename T>
T *Object::GetOrAddComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    auto component = GetComponent<T>();
    if (component != nullptr) {
        return component;
    }

    return AddComponent<T>();
}

template<typename T>
void Object::RemoveComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    auto t = std::type_index(typeid(T));
    auto it = components.find(t);
    if (it == components.end()) {
        LOG_WARN("Object", "Component %s does not exist.", typeid(T).name());
        return;
    }

    components.erase(it);
}

template<typename T>
bool Object::HasComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    auto t = std::type_index(typeid(T));
    return (components.find(t) != components.end());
}