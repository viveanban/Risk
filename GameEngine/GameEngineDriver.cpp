#include "GameEngine.h"
#include <ctime>
#include <algorithm>

using namespace std;

int main() {
    srand(time(0));

    GameEngine* gameEngine = GameEngine::getInstance();
    gameEngine->startupPhase();
    gameEngine->mainGameLoop();

    delete gameEngine;

    return 0;
}