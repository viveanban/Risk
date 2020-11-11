#include "./../GameEngine/GameEngine.h"

using namespace std;

int main() {
    GameEngine gameEngine;
    gameEngine.mainGameLoop();
    // a player receives the correct number of armies in the reinforcement phase (showing different cases)

    // a player will only issue deploy orders and no other kind of orders if they still have armies in their reinforcement pool

    // the game engine will only execute non-deploy orders when all the deploy orders of all players have been executed

    // a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists

    // a player can play cards to issue orders

    // a player that does not control any territory is removed from the game

    // the game ends when a single player controls all the territories
}
