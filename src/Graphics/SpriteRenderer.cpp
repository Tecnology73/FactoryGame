#include "SpriteRenderer.h"
#include "../Engine/Object.h"
#include <raylib.h>

#include <utility>

void SpriteRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
    if (currentAnimation.empty()) return;

    const Animation &animation = animations[currentAnimation];
    if (++frameCounter >= (GetFPS() / animation.playbackFps)) {
        frameCounter = 0;
        currentFrame++;

        if (currentFrame >= animation.frames.size()) currentFrame = 0;

        auto frameIndex = animation.frames[currentFrame];
        sourceRec.position.x = (float) (frameIndex % framesPerColumn) * frameSize.x;
        sourceRec.position.y = (float) (frameIndex / framesPerColumn) * frameSize.y;
    }
}

void SpriteRenderer::Render() {
    Component::Render();

    auto pos = object->GetTransform()->position;
    DrawTextureRec(texture->GetTexture(),
                   {sourceRec.position.x, sourceRec.position.y, sourceRec.size.x, sourceRec.size.y}, {pos->x, pos->y},
                   WHITE);
}

void SpriteRenderer::SetFrameLayout(uint32_t columns, uint32_t rows) {
    framesPerColumn = columns;
    framesPerRow = rows;
    frameSize.x = texture->GetTexture().width / columns;
    frameSize.y = texture->GetTexture().height / rows;
    sourceRec = {0, 0, frameSize.x, frameSize.y};
}

bool SpriteRenderer::AddAnimation(const Animation &animation) {
    if (animations.contains(animation.name)) return false;

    animations.emplace(animation.name, animation);
    return true;
}

bool SpriteRenderer::PlayAnimation(const std::string &name) {
    if (!animations.contains(name)) {
        LOG_DEBUG("SpriteRenderer", "Animation %s not found on %s (%llu)", name.c_str(), object->GetName().c_str(),
                  object->GetId());
        return false;
    }

    currentAnimation = name;
    currentFrame = 0;
    frameCounter = 0;

    auto frameIndex = animations[name].frames[0];
    sourceRec.position.x = (float) (frameIndex % framesPerColumn) * frameSize.x;
    sourceRec.position.y = (float) (frameIndex / framesPerColumn) * frameSize.y;
    return true;
}

void SpriteRenderer::SetTexture(SharedPtr<Texture> texture) {
    this->texture = std::move(texture);
}
