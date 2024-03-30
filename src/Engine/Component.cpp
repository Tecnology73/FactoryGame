#include "Component.h"
#include "Object.h"

Component::Component(Object *owner) :
    object(owner) {
}

void Component::Start() {}

void Component::Update(float deltaTime) {}

void Component::OnDestroy() {}

void Component::Render() {}

void Component::RenderDebug() {}

bool Component::IsActive() const {
    return isActive;
}

void Component::SetActive(bool value) {
    isActive = value;
}