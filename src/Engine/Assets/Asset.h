#pragma once

class IAsset {
public:
    virtual ~IAsset() = default;

    virtual void Load(const std::string &path) = 0;
};