#include "Player.h"
#include "../Networking/Packets/PlayerPositionUpdatePacket.h"
#include <raylib.h>

void Player::Update(float deltaTime) {
    NetworkObject::Update(deltaTime);

    if (!IsOwner()) {
        return;
    }

    const float moveSpeed = IsKeyDown(KEY_LEFT_SHIFT) ? 20.f : 10.f;

    auto transform = GetTransform();
    float x = transform->position->x;
    float y = transform->position->y;
    if (IsKeyDown(KEY_W)) y -= moveSpeed * deltaTime;
    if (IsKeyDown(KEY_S)) y += moveSpeed * deltaTime;
    if (IsKeyDown(KEY_A)) x -= moveSpeed * deltaTime;
    if (IsKeyDown(KEY_D)) x += moveSpeed * deltaTime;

    transform->position = {x, y};
}

void Player::NetworkUpdate() {
    NetworkObject::NetworkUpdate();
}

void Player::Render() {
    Object::Render();

    auto transform = GetTransform();
    DrawRectangleV(
        {transform->position->x, transform->position->y},
        {transform->size->x, transform->size->y},
        IsOwner() ? BLUE : RED
    );
}