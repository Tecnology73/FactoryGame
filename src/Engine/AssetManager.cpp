#include "AssetManager.h"

AssetManager *AssetManager::instance = nullptr;

AssetManager::AssetManager() {
    instance = this;
}