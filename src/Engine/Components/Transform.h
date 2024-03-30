#pragma once

#include "../Component.h"
#include "../Common.h"
#include "../Observable.h"

namespace MyEngine {
    class Transform : public Component {
        using Component::Component;

    public:
        bool IsDirty() const {
            return position.IsDirty() || size.IsDirty() || rotation.IsDirty();
        }

        Rectangle2D GetRect() const { return {position->x, position->y, size->x, size->y}; }

    public:
        Observable<Vec2> position;
        Observable<Vec2> size{{10.f, 10.f}};
        Observable<float> rotation;
    };
}
