#pragma once

#include <string>
#include <cstdint>
#include <utility>
#include "../Util.h"
#include "../Engine/Component.h"
#include "../Engine/Common.h"
#include "../Engine/Assets/Texture.h"
#include "Animation.h"
#include <rcommon.h>

class SpriteRenderer : public Component {
public:
    using Component::Component;

    explicit SpriteRenderer(Object *owner) : Component(owner),
                                             currentFrame(0),
                                             frameCounter(0) {}

    void Update(float deltaTime) override;

    void Render() override;

    void SetTexture(SharedPtr<Texture> texture);

    void SetFrameLayout(uint32_t columns, uint32_t rows);

    bool AddAnimation(const Animation &animation);

    bool PlayAnimation(const std::string &name);

private:
    SharedPtr<Texture> texture;
    uint32_t currentFrame;
    uint32_t frameCounter;
    uint32_t framesPerColumn;
    uint32_t framesPerRow;
    Rectangle2D sourceRec;
    Vec2 frameSize;

    UnorderedMap<std::string, Animation> animations;
    std::string currentAnimation;
};