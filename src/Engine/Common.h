#pragma once

#include "../Networking/ByteStream.h"

struct ISerializable {
    virtual void Serialize(ByteStream &stream) const = 0;

    virtual void Deserialize(ByteStream &stream) = 0;
};

struct Vec2 : ISerializable {
    float x;
    float y;

    Vec2() : x(0), y(0) {}

    Vec2(float x, float y) : x(x), y(y) {}

    void Serialize(ByteStream &stream) const override {
        stream.Write(x);
        stream.Write(y);
    }

    void Deserialize(ByteStream &stream) override {
        stream.Read(x);
        stream.Read(y);
    }
    
    bool operator==(const Vec2 &other) const {
        return x == other.x && y == other.y;
    }

    Vec2 operator+(const Vec2 &other) const {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2 &other) const {
        return {x - other.x, y - other.y};
    }

    Vec2 operator+=(const Vec2 &other) const {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-=(const Vec2 &other) const {
        return {x - other.x, y - other.y};
    }
};

struct Rectangle2D : ISerializable {
    Vec2 position;
    Vec2 size;

    Rectangle2D() : position(), size() {}

    Rectangle2D(float x, float y, float width, float height) : position({x, y}), size({width, height}) {}

    void Serialize(ByteStream &stream) const override {
        position.Serialize(stream);
        size.Serialize(stream);
    }

    void Deserialize(ByteStream &stream) override {
        position.Deserialize(stream);
        size.Deserialize(stream);
    }
};
