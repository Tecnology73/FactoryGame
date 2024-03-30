#pragma once

#include "../Engine/Component.h"
#include "../Util.h"

class PlayerMovement : public Component {
public:
    using Component::Component;

    float t;

    void Update(float deltaTime) override {
        LOG_INFO("PlayerMovement", "Update %f", t);

        t += deltaTime;
        if (t > 2.f) {
            SetActive(false);
        }
    }
};