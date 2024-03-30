#pragma once

#include <string>
#include <rcommon.h>
#include "Asset.h"

class Texture : public IAsset {
public:
    ~Texture() override;

    void Load(const std::string &path) override;

    RTexture2D GetTexture() const;

private:
    RTexture2D texture;
};