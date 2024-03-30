#pragma once

#include "../Util.h"

struct Animation {
    std::string name;
    uint32_t playbackFps;
    Vector<uint32_t> frames;

    Animation() = default;

    Animation(std::string name, uint32_t startFrame, uint32_t numFrames, uint32_t playbackFps) :
        name(std::move(name)),
        playbackFps(playbackFps) {
        frames.reserve(numFrames);
        for (uint32_t i = 0; i < numFrames; i++) {
            frames.push_back(startFrame + i);
        }
    }

    Animation(std::string name, Vector<uint32_t> frames, uint32_t playbackFps) :
        name(std::move(name)),
        frames(std::move(frames)),
        playbackFps(playbackFps) {}
};