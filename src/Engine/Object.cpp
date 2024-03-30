#include "Object.h"
#include <raylib.h>
#include "EntityManager.h"

void Object::Init() {}

void Object::Update(float deltaTime) {
    for (auto &component: components) {
        if (!component.second->IsActive()) continue;
        component.second->Update(deltaTime);
    }
}

void Object::Render() {
    for (auto &component: components) {
        if (!component.second->IsActive()) continue;
        component.second->Render();
    }
}

void Object::DrawDebug() {
    auto transform = GetTransform();
    DrawRectangleLinesEx({transform->position->x, transform->position->y, transform->size->x, transform->size->y}, 1, RED);

    auto textWidth = MeasureText(name.c_str(), 10);
    DrawText(
        name.c_str(),
        transform->position->x + (transform->size->x / 2) - (textWidth / 2),
        transform->position->y + (transform->size->y / 2) + 5,
        10,
        LIGHTGRAY
    );
}

void Object::OnDestroy() {}

void Object::Destroy() {
    LOG_DEBUG("Object", "Destroying object %llu", id);
    OnDestroy();
    EntityManager::Destroy(id);
}

bool Object::IsActive() const {
    return isActive;
}

void Object::SetActive(bool value) {
    isActive = value;
    if (isActive) {
        Init();
    }
}
