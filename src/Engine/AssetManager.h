#pragma once

#include "../Util.h"
#include "../Engine/ISystem.h"
#include "Assets/Asset.h"

class AssetManager : public ISystem {
public:
    AssetManager();

    static std::string GetName() { return "Asset Manager"; }

    int Init() override { return 0; }

    template<typename T>
    static SharedPtr<T> Load(const std::string &path) {
        static_assert(std::is_base_of_v<IAsset, T>, "T must inherit from IAsset");

        auto hash = std::hash<std::string>{}(path);
        auto it = instance->assets.find(hash);
        if (it != instance->assets.end()) {
            return std::static_pointer_cast<T>(it->second);
        }

        auto asset = MakeShared<T>();
        asset->Load(path);
        instance->assets[hash] = asset;
        return asset;
    }
    
private:
    static AssetManager *instance;

    UnorderedMap<uint64_t, SharedPtr<IAsset>> assets;
};