#pragma once

class Object;

class Component {
public:
    explicit Component(Object *owner);

    virtual ~Component() = default;

    virtual void Start();

    virtual void Update(float deltaTime);

    virtual void OnDestroy();

    virtual void Render();

    virtual void RenderDebug();

    [[nodiscard]] bool IsActive() const;

    void SetActive(bool value);

protected:
    Object *object = nullptr;
    bool isActive = true;
};