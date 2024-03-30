#pragma once

#include <string>

class ISystem {
public:
    virtual ~ISystem() = default;

    [[nodiscard]] static std::string GetName() { return "ISystem"; }

    [[nodiscard]] virtual int Init() { return 0; }

    virtual void Start() {}

    virtual void Shutdown() {}

    virtual void PreUpdate(float deltaTime) {}

    virtual void Update(float deltaTime) {}

    virtual void PostUpdate(float deltaTime) {}
};