#include <iostream>
#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <algorithm>

/**
 * Player Class implementation
 */

Player *Player::neutralPlayer = new Player("Neutral Player");

Player::Player(string playerName) : playerName(playerName), handOfCards(new Hand()),
                                    orders(new OrdersList()),
                                    numberOfArmiesInReinforcementPool(0), territories(),
                                    strategy(nullptr) {}

Player::~Player() {
    delete handOfCards;
    handOfCards = nullptr;
    cout << "Deleted Hand" << endl;

    delete orders;
    orders = nullptr;
    cout << "Deleted orders" << endl;

    delete strategy;
    strategy = nullptr;
    cout << "Deleted strategy" << endl;
}

Player::Player(const Player &original) {
    playerName = original.playerName;
    territories = vector<Territory *>(original.territories.size());
    for (int i = 0; i < territories.size(); i++) {
        territories[i] = original.territories[i];
    }
    numberOfArmiesInReinforcementPool = original.numberOfArmiesInReinforcementPool;
    handOfCards = new Hand(*original.handOfCards);
    orders = new OrdersList(*original.orders);
}

Player &Player::operator=(const Player &otherPlayer) {
    if (&otherPlayer != this) {
        playerName = otherPlayer.playerName;
        territories = vector<Territory *>(otherPlayer.territories.size());
        for (int i = 0; i < territories.size(); i++) {
            territories[i] = otherPlayer.territories[i];
        }
        numberOfArmiesInReinforcementPool = otherPlayer.numberOfArmiesInReinforcementPool;

        //Deleting old values in heap
        delete handOfCards;
        delete orders;

        //Making a deep copy of new ones
        handOfCards = new Hand(*otherPlayer.handOfCards);
        orders = new OrdersList(*otherPlayer.orders);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &stream, Player &player) {
    return stream << "Information on Player object:" << endl
                  << "Address: " << &player << endl
                  << "Player Name: " << player.getPlayerName() << endl
                  << "Number of Territories Owned: " << player.getTerritories().size() << endl
                  << "Size of Hand: " << player.getHandOfCards()->getCards().size() << endl
                  << "Number of Orders: " << player.getOrders()->getOrderList().size() << endl
                  << "Number of Armies: " << player.numberOfArmiesInReinforcementPool << endl;
}

// Cannot call the setOwner method from here
void Player::addTerritory(Territory *territory) {
    territories.push_back(territory);
}

// Cannot call the setOwner method from here
void Player::removeTerritory(Territory *territory) {
    auto position = find(territories.begin(), territories.end(), territory);
    if (position != territories.end()) {
        territories.erase(position);
    }
}

vector<Territory *> Player::toDefend() {
    return this->strategy->toDefend();
}

vector<Territory *> Player::toDefend(Territory *srcTerritory) {
    return this->strategy->toDefend(srcTerritory);
}

vector<Territory *> Player::toAttack() {
    return this->strategy->toAttack();
}

vector<Territory *> Player::toAttack(Territory *srcTerritory) {
    return this->strategy->toAttack(srcTerritory);
}

// TODO: where is this used?
void Player::sortTerritoryList(vector<Territory *> &territoryList) {
    sort(territoryList.begin(), territoryList.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
}

bool Player::issueOrder() {
    return this->strategy->issueOrder();
}

// Getters
string Player::getPlayerName() {
    return this->playerName;
}

vector<Territory *> &Player::getTerritories() {
    return this->territories;
}

Hand *Player::getHandOfCards() {
    return this->handOfCards;
}

OrdersList *Player::getOrders() {
    return this->orders;
}

int Player::getNumberofArmiesInReinforcementPool() {
    return this->numberOfArmiesInReinforcementPool;
}

set<Player *> &Player::getPlayersNotToAttack() {
    return playersNotToAttack;
}

// Setters
void Player::setNumberOfArmiesInReinforcementPool(int numberOfArmiesInReinforcementPool) {
    this->numberOfArmiesInReinforcementPool = numberOfArmiesInReinforcementPool;
}

void Player::setStrategy(PlayerStrategy *playerStrategy) {
    this->strategy = playerStrategy;
}

PlayerStrategy *Player::getStrategy() const {
    return strategy;
}