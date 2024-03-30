#include "GameManager.h"
#include "../Engine/EntityManager.h"
#include "../Engine/Object.h"
#include "../Engine/AssetManager.h"
#include <raylib.h>

int GameManager::Init() {
    return 0;
}

void GameManager::Start() {
    auto obj = Object::Create({100, 100});
    sprite = obj->AddComponent<SpriteRenderer>();
    sprite->SetTexture(AssetManager::Load<Texture>("../assets/player.png"));
    // sprite->Load("../assets/player.png");
    sprite->SetFrameLayout(22, 1);
    sprite->AddAnimation({"attack", 0, 4, 10});
    sprite->AddAnimation({"crouch", 4, 4, 10});
    sprite->AddAnimation({"roll", 8, 4, 10});
    sprite->AddAnimation({"run", 12, 8, 10});
    sprite->AddAnimation({"slide", 20, 2, 10});
    // sprite->AddAnimation({"run", {0, 1, 2, 3, 4, 5, 6, 7}, 10});
    sprite->PlayAnimation("run");
}

void GameManager::PreUpdate(float deltaTime) {
    static int id = 0;
    if (IsKeyPressed(KEY_SPACE)) {
        switch (id++) {
            case 0:
                sprite->PlayAnimation("attack");
                break;
            case 1:
                sprite->PlayAnimation("crouch");
                break;
            case 2:
                sprite->PlayAnimation("roll");
                break;
            case 3:
                sprite->PlayAnimation("run");
                break;
            case 4:
                sprite->PlayAnimation("slide");
                break;
            default:
                id = 0;
                break;
        }
    }
}
