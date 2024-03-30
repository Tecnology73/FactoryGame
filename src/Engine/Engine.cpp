#include "Engine.h"
#include "../Networking/ServerNetworkInterface.h"
#include "../Networking/ClientNetworkInterface.h"
#include "../Networking/NetworkObjectFactory.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "Object.h"
#include <raylib.h>

Engine *Engine::instance = nullptr;
uint64_t Engine::frameNumber = 0;

Engine::Engine() {
    instance = this;
}

void Engine::SetRunType(Engine::RunType value) {
    runType = value;
}

void Engine::Run(const Vec2 &windowSize, const char *windowTitle, uint16_t fps) {
    CreateNetworkStack();
    CreateGameWindow(windowSize, windowTitle, fps);
    InitCoreSystems();
    RunGameLoop();
}

void Engine::Shutdown() {
    for (const auto &item: coreSystems) {
        item.second->Shutdown();
    }

    CloseWindow();
}

void Engine::CreateNetworkStack() {
    switch (runType) {
        case RunType::Client:
            network = RegisterSystem<ClientNetworkInterface>();
            break;
        case RunType::Server:
        case RunType::DedicatedServer:
            network = RegisterSystem<ServerNetworkInterface>();
            break;
    }

    RegisterSystem<NetworkObjectFactory>();
}

void Engine::CreateGameWindow(const Vec2 &windowSize, const char *title, int fps) {
    InitWindow(windowSize.x, windowSize.y, title);
    SetTargetFPS(currentTargetFps = fps);

    graphics = RegisterSystem<Graphics>();
    RegisterSystem<EntityManager>();
}

void Engine::InitCoreSystems() {
    RegisterSystem<AssetManager>();

    for (const auto &item: coreSystems) {
        int result = item.second->Init();
        if (result != 0) {
            LOG_ERROR("Engine", "Failed to initialize system \"%s\": %d", item.second->GetName().c_str(), result);
            exit(result);
        }
    }
}

void Engine::RunGameLoop() {
    using deltaClock = std::chrono::high_resolution_clock;
    deltaClock::time_point previousPreUpdate;
    deltaClock::time_point previousPostUpdate;

    // Call Start on all the coreSystems
    for (const auto &item: coreSystems) {
        item.second->Start();
    }

    // Actual game loop.
    while (!WindowShouldClose()) {
        // We don't care if this overflows.
        // At 240fps, it'd take 2.4 billion years to overflow 😄
        frameNumber++;

        // PreUpdate
        {
            auto now = deltaClock::now();
            auto duration = now - previousPreUpdate;
            auto deltaTime = std::chrono::duration<float>(duration).count();

            PreUpdate(deltaTime);
            previousPreUpdate = now;
        }

        // Update
        {
            float deltaTime = GetFrameTime();
            ProcessInput();
            Update(deltaTime);

            if (!network->IsReady()) {
                DrawText("Waiting for network...", 10, 10, 20, WHITE);
            } else {
                graphics->Update(deltaTime);
            }
        }

        // PostUpdate
        {
            auto now = deltaClock::now();
            auto duration = now - previousPostUpdate;
            auto deltaTime = std::chrono::duration<float>(duration).count();

            PostUpdate(deltaTime);
            previousPostUpdate = now;
        }
    }
}

void Engine::ProcessInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
        return;
    }

    if (IsKeyPressed(KEY_MINUS)) {
        currentTargetFps -= 10;
        SetTargetFPS(currentTargetFps);
    }
    if (IsKeyPressed(KEY_EQUAL)) {
        currentTargetFps += 10;
        SetTargetFPS(currentTargetFps);
    }
}

void Engine::PreUpdate(float deltaTime) {
    // LOG_DEBUG("Engine", "PreUpdate: %f", deltaTime);
    for (const auto &item: coreSystems) {
        item.second->PreUpdate(deltaTime);
    }
}

void Engine::Update(float deltaTime) {
    // LOG_DEBUG("Engine", "Update: %f", deltaTime);
    for (const auto &item: EntityManager::GetObjects()) {
        if (!item.second->IsActive()) continue;
        item.second->Update(deltaTime);
    }
}

void Engine::PostUpdate(float deltaTime) {
    // LOG_DEBUG("Engine", "PostUpdate: %f", deltaTime);
    for (const auto &item: coreSystems) {
        item.second->PostUpdate(deltaTime);
    }
}