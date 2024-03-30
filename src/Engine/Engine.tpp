#pragma once

template<typename T>
T *Engine::RegisterSystem() {
    static_assert(std::is_base_of<ISystem, T>::value, "T must inherit from ISystem.");

    auto index = std::type_index(typeid(T));
    if (coreSystems.find(index) != coreSystems.end()) {
        LOG_ERROR("Engine", "System \"%s\" already registered.", T::GetName().c_str());
        return nullptr;
    }

    auto system = std::make_unique<T>();
    auto ptr = system.get();
    coreSystems[index] = std::move(system);

    return ptr;
}

template<typename T>
T *Engine::GetSystem() {
    static_assert(std::is_base_of<ISystem, T>::value, "T must inherit from ISystem.");

    auto index = std::type_index(typeid(T));
    auto it = coreSystems.find(index);
    if (it == coreSystems.end()) {
        LOG_ERROR("Engine", "System \"%s\" not registered.", T::GetName().c_str());
        return nullptr;
    }

    return dynamic_cast<T *>(it->second.get());
}