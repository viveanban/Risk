#include "GameEngine.h"
#include <set>
#include <algorithm>
#include <ctime>

void GameEngine::mainGameLoop() {
    srand(time(0)); // TODO: remove and put in driver

    while (!winnerExists()) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        removePlayersWithoutTerritoriesOwned();
    }

}

void GameEngine::reinforcementPhase() {
    for (Player *player: players) {
        int numberOfArmiesToGive = calculateNumberOfArmiesToGive(player);
        player->setNumberOfArmies(numberOfArmiesToGive);
    }
}

int GameEngine::calculateNumberOfArmiesToGive(Player *player) {
    int numberOfArmiesToGive = player->getTerritories().size() / 3;
    if (numberOfArmiesToGive >= 0 && numberOfArmiesToGive <= 2)
        numberOfArmiesToGive = 3;

    // Bonus
    set<Continent *> continentsWherePlayerOwnsTerritories;
    for (Territory *territory: player->getTerritories()) {
        int continentId = territory->getContinentId();
        continentsWherePlayerOwnsTerritories.insert(map->getContinentList().at(continentId - 1));
    }

    int controlValueBonus = 0;
    for (Continent *continent: continentsWherePlayerOwnsTerritories) {
        if (continent->getOwner() == player)
            controlValueBonus += continent->getBonus();
    }

    return numberOfArmiesToGive + controlValueBonus;
}

void GameEngine::issueOrdersPhase() {
    vector<Player *> playersWithNoMoreOrderstoIssue;
    while (playersWithNoMoreOrderstoIssue.size() != players.size()) {
        for (Player *player: players) {
            if (find(playersWithNoMoreOrderstoIssue.begin(), playersWithNoMoreOrderstoIssue.end(), player) ==
                playersWithNoMoreOrderstoIssue.end()) {
                if (!player->issueOrder())
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

    // Execute all deploy orders
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

    // Execute the rest of the orders
    vector<Player *> playersWithNoMoreOrdersToExecute;
    while (playersWithNoMoreOrdersToExecute.size() != players.size()) {
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

bool GameEngine::winnerExists() {
    return players.size() == 1;
}

void GameEngine::removePlayersWithoutTerritoriesOwned() {
    for (Player *player: players) {
        if(player->getTerritories().empty()) {
            auto position = find(players.begin(), players.end(), player);
            if (position != players.end()) {
                players.erase(position);
                delete player;
                player = nullptr;
            }
        }
    }
}
