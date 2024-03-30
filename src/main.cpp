#include <iostream>
#include "Engine/Engine.h"
#include "Game/GameManager.h"

Engine::RunType getRunType() {
    int userChoice = 0;
    while (userChoice != 1 && userChoice != 2 && userChoice != 3) {
        std::cout << "1. Client" << std::endl;
        std::cout << "2. Host Server" << std::endl;
        std::cout << "3. Dedicated Server" << std::endl;
        std::cout << "Choice: ";
        std::cin >> userChoice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            userChoice = 0;
        }
    }

    return static_cast<Engine::RunType>(userChoice);
}

int main() {
    auto engine = new Engine();
    engine->SetRunType(getRunType());

    // Register custom systems here.
    engine->RegisterSystem<GameManager>();

    engine->Run({1024, 768}, "Game", 240);
    engine->Shutdown();
    return 0;
}