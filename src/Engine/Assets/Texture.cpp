#include "Texture.h"
#include "../../Util.h"
#include <raylib.h>

Texture::~Texture() {
    UnloadTexture(texture);
}

void Texture::Load(const std::string &path) {
    texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        LOG_ERROR("Texture", "Failed to load texture from %s", path.c_str());
    }
}

RTexture2D Texture::GetTexture() const {
    return texture;
}
