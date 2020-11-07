#include "GameEngine.h"

void GameEngine::mainGameLoop() {
    while(!winnerExists()) {


        removePlayersWithoutTerritoriesOwned();
    }

}

void GameEngine::reinforcementPhase() {
    // TODO: loop through all the players

    Player* currentPlayer = this->players.at(0); // TODO: change that

    int numberOfTerritoriesOwned = currentPlayer->getTerritories().size();
    int numberOfArmiesToGive = numberOfTerritoriesOwned/3; // TODO: does it round down (floor) always?

    if(playerOwnsEntireContinent()) {     // TODO: efficient way to check if player owns entire continent?
        // retrieve said continent's control value
        int continentControlBonusValue = 0;
        numberOfArmiesToGive += continentControlBonusValue;
    }

    if(numberOfArmiesToGive == 0 || numberOfArmiesToGive == 1 || numberOfArmiesToGive == 2)
        currentPlayer->setNumberOfArmies(3);
    else
        currentPlayer->setNumberOfArmies(numberOfArmiesToGive);

}

void GameEngine::issueOrdersPhase() {
    // TODO: loop through all the players

    Player* currentPlayer = this->players.at(0); // TODO: change that
    currentPlayer->issueOrder();


}

void GameEngine::executeOrdersPhase() {

}

// TODO: implement
bool GameEngine::winnerExists() {
    return false;
}

// TODO: implement
void GameEngine::removePlayersWithoutTerritoriesOwned() {

};
