#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <memory>
#include "Common.h"
#include "../Util.h"
#include "ISystem.h"
#include "Graphics.h"
#include "../Networking/NetworkCore.h"

class Engine {
public:
    static uint64_t frameNumber;

    enum class RunType : uint8_t {
        Client = 1, // RemoteClient connecting to a remote server.
        Server = 2, // LocalServer with a local player.
        DedicatedServer = 3, // LocalServer with no local player.
    };

    Engine();

    void SetRunType(RunType value);

    void Run(const Vec2 &windowSize, const char *windowTitle, uint16_t fps);

    void Shutdown();

    template<typename T>
    T *RegisterSystem();

    template<typename T>
    T *GetSystem();

    static Engine *Get() { return instance; }

protected:
    void ProcessInput();

    void PreUpdate(float deltaTime);

    void Update(float deltaTime);

    void PostUpdate(float deltaTime);

private:
    void CreateNetworkStack();

    void CreateGameWindow(const Vec2 &windowSize, const char *title, int fps);

    void InitCoreSystems();

    void RunGameLoop();

private:
    static Engine *instance;

    RunType runType = RunType::Server;
    std::unordered_map<std::type_index, std::unique_ptr<ISystem>> coreSystems;

    // Systems that'll always be present.
    // Here so we don't need to go through the map for them.
    Graphics *graphics = nullptr;
    NetworkCore *network = nullptr;

    int currentTargetFps = 240;
};

#include "Engine.tpp"