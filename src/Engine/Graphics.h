#pragma once

#include "ISystem.h"

class Graphics : public ISystem {
public:
    [[nodiscard]] static std::string GetName() { return "Graphics"; }

    [[nodiscard]] int Init() override;

    void Update(float deltaTime) override;

private:
    void DrawGrid();
};