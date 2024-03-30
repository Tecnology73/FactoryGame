#pragma once

#include <string>
#include <utility>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include "Common.h"
#include "../Util.h"
#include "Component.h"
#include "Components/Transform.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    explicit Object(uint64_t id, const Vec2 &position) : id(id) {
        AddComponent<MyEngine::Transform>()->position = position;
    }

    Object() = delete;

    virtual ~Object() = default;

    template<typename T = Object>
    static SharedPtr<T> Create(const Vec2 &position);

    virtual void Init();

    virtual void OnDestroy();

    virtual void Update(float deltaTime);

    virtual void Render();

    virtual void DrawDebug();

    void Destroy();

    /**
     * Component Functions
     */
public:
    template<typename T>
    T *AddComponent();

    template<typename T>
    T *GetComponent() const;

    template<typename T>
    T *GetOrAddComponent();

    template<typename T>
    void RemoveComponent();

    template<typename T>
    bool HasComponent() const;

    /**
     * Property Functions
     */
public:
    uint64_t GetId() const { return id; }

    std::string GetName() const { return name; }

    virtual void SetName(const std::string &value) { name = value; }

    bool IsActive() const;

    void SetActive(bool value);

    MyEngine::Transform *GetTransform() const { return GetComponent<MyEngine::Transform>(); }

protected:
    const uint64_t id;
    std::string name;
    bool isActive = true;

    UnorderedMap<std::type_index, UniquePtr<Component>> components;
};

#include "Object.tpp"