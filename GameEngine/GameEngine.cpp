#include "GameEngine.h"
#include <set>
#include <algorithm>

void GameEngine::mainGameLoop() {

    while(!winnerExists()) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        removePlayersWithoutTerritoriesOwned();
    }

}

void GameEngine::reinforcementPhase() {
    for(Player* player: players) {

        int numberOfTerritoriesOwned = player->getTerritories().size();
        int numberOfArmiesToGive = numberOfTerritoriesOwned/3; // TODO: does it round down (floor) always?
        if(numberOfArmiesToGive >= 0 && numberOfArmiesToGive <= 2)
            numberOfArmiesToGive = 3;

        int controlValueBonus = 0;
        set<Continent *> continentsWherePlayerOwnsTerritories;
        for(Territory* territory: player->getTerritories()) {
            int continentId = territory->getContinentId();
            continentsWherePlayerOwnsTerritories.insert(map->getContinentList().at(continentId - 1));
        }

        for(Continent* continent: continentsWherePlayerOwnsTerritories) {
            if (continent->getOwner() == player)
                controlValueBonus += continent->getBonus();
        }

        numberOfArmiesToGive += controlValueBonus;

        player->setNumberOfArmies(numberOfArmiesToGive);
    }
}

void GameEngine::issueOrdersPhase() {
    vector<Player*> playersWithNoMoreOrderstoIssue;
    bool playerStillIssuingOrders;

    while(playersWithNoMoreOrderstoIssue.size() != players.size()) {
        for(Player* player: players) {
            if(find(playersWithNoMoreOrderstoIssue.begin(), playersWithNoMoreOrderstoIssue.end(), player) == playersWithNoMoreOrderstoIssue.end()) {
                playerStillIssuingOrders = player->issueOrder();

                if(!playerStillIssuingOrders)
                    playersWithNoMoreOrderstoIssue.push_back(player);
            }
        }
    }
}

// TODO: implement
void GameEngine::executeOrdersPhase() {

}

// TODO: implement
bool GameEngine::winnerExists() {
    // keep state in player
    // or check territories of each player
    return false;
}

// TODO: implement
void GameEngine::removePlayersWithoutTerritoriesOwned() {

}
