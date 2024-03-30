#pragma once

#include "../Util.h"
#include "Engine.h"

template<typename T>
class Observable {
public:
    explicit Observable(const T &value = T()) : value(value) {}

    T Get() const {
        return value;
    }

    operator T() const {
        return value;
    }

    T *operator->() {
        return &value;
    }

    const T *operator->() const {
        return &value;
    }

    Observable<T> &operator=(const T &newValue) {
        if (value != newValue) {
            value = newValue;
            MarkDirty();
        }

        return *this;
    }

    bool operator==(const T &other) const {
        return value == other;
    }

    bool operator!=(const T &other) const {
        return value != other;
    }

    Observable<T> &operator+=(const T &other) {
        value += other;
        MarkDirty();
        return *this;
    }

    Observable<T> &operator-=(const T &other) {
        value -= other;
        MarkDirty();
        return *this;
    }

    bool IsDirty() const {
        // If the dirty frame is the current frame, then it's dirty.
        // NOTE: This means that we can't have an Observable that is dirty for more than one frame.
        return dirtyFrame >= Engine::frameNumber;
    }

    void MarkDirty() {
        dirtyFrame = Engine::frameNumber + 1;
    }

    void ClearDirty() {
        // The dirty flag would automatically clear itself each frame, but we can force it to clear.
        dirtyFrame = 0;
    }

private:
    T value;
    uint64_t dirtyFrame = 0;
};