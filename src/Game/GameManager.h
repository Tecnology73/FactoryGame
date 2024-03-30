#pragma once

#include "../Engine/ISystem.h"
#include "../Graphics/SpriteRenderer.h"

class GameManager : public ISystem {
public:
    [[nodiscard]] static std::string GetName() { return "Graphics"; }

    [[nodiscard]] int Init() override;

    void Start() override;

    void PreUpdate(float deltaTime) override;

private:
    SpriteRenderer *sprite;
};