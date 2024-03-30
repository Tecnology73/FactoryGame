#include "Graphics.h"
#include "EntityManager.h"
#include "Object.h"
#include <raylib.h>

int Graphics::Init() {
    return 0;
}

void Graphics::Update(float deltaTime) {
    static bool isSceneView = false;
    if (IsKeyPressed(KEY_GRAVE)) {
        isSceneView = !isSceneView;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    if (isSceneView) {
        DrawGrid();
    }

    for (const auto &item: EntityManager::GetObjects()) {
        item.second->Render();

        if (isSceneView) {
            item.second->DrawDebug();
        }
    }

    DrawFPS(10, 10);
    DrawText(TextFormat("Scene View: %s", isSceneView ? "On" : "Off"), 10, 30, 10, LIGHTGRAY);
    DrawText(TextFormat("Network Type: %s", ConnectionManager::IsServer() ? "Server" : "Client"), 10, 50, 10, LIGHTGRAY);
    DrawText(TextFormat("Network ID: %u", NetworkCore::GetNetworkId()), 10, 70, 10, LIGHTGRAY);

    EndDrawing();
}

void Graphics::DrawGrid() {
    static const int gridSize = 50;
    static const int gridOverscan = 2;

    float startX = 0; // ((int) World.Camera.target.x / gridSize) * gridSize;
    float startY = 0; // ((int) World.Camera.target.y / gridSize) * gridSize;
    float screenWidth = GetRenderWidth() / 1.f; // World.Camera.zoom;
    float screenHeight = GetRenderHeight() / 1.f; // World.Camera.zoom;

    for (int i = 0; i < screenWidth / gridSize + gridOverscan; i++) {
        DrawLine(
            startX + (i * gridSize),
            startY - gridSize,
            startX + (i * gridSize),
            startY + screenHeight + gridSize,
            LIGHTGRAY
        );
    }

    for (int i = 0; i < screenHeight / gridSize + gridOverscan; i++) {
        DrawLine(
            startX - gridSize,
            startY + (i * gridSize),
            startX + screenWidth + gridSize,
            startY + (i * gridSize),
            LIGHTGRAY
        );
    }
}
