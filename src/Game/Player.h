#pragma once

#include <utility>
#include <cstddef>
#include "../Networking/NetworkObject.h"

class Player : public NetworkObject {
public:
    Player(uint64_t id, const Vec2 &position) : NetworkObject(id, position) {}

    void Update(float deltaTime) override;

    void NetworkUpdate() override;

    void Render() override;
};