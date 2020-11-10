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
    while(playersWithNoMoreOrderstoIssue.size() != players.size()) {
        for(Player* player: players) {
            if(find(playersWithNoMoreOrderstoIssue.begin(), playersWithNoMoreOrderstoIssue.end(), player) == playersWithNoMoreOrderstoIssue.end()) {
                if(!player->issueOrder())
                    playersWithNoMoreOrderstoIssue.push_back(player);
            }
        }
    }
}

void GameEngine::executeOrdersPhase() {
    // Prioritize the orders
    for (Player *player: players) {
        player->getOrders()->sortOrderListByPriority();
    }

    //Execute all deploy orders
    vector<Player *> playersWithNoMoreDeployOrderstoExecute;
    while (playersWithNoMoreDeployOrderstoExecute.size() != players.size()) {
        for (Player *player: players) {
            vector<Order *> &orderList = player->getOrders()->getOrderList();
            if (!orderList.empty()) {
                auto *deployOrder = dynamic_cast<DeployOrder *>(orderList[0]);
                if (deployOrder) {
                    deployOrder->execute();
                    player->getOrders()->remove(deployOrder);
                } else {
                    playersWithNoMoreDeployOrderstoExecute.push_back(player);
                }
            }
        }
    }

    //Execute the rest of the orders
    vector<Player *> playersWithNoMoreOrdersToExecute;
    while (playersWithNoMoreDeployOrderstoExecute.size() != players.size()) {
        for (Player *player: players) {
            vector<Order *> &orderList = player->getOrders()->getOrderList();
            if (!orderList.empty()) {
                orderList[0]->execute();
                player->getOrders()->remove(orderList[0]);
            } else {
                playersWithNoMoreOrdersToExecute.push_back(player);
            }
        }
    }
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
