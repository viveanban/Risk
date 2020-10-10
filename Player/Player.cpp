#include <iostream>
#include "Player.h"
#include <algorithm>

/**
 * Player Class implementation
 */
Player::Player() : playerName(), handOfCards(new Hand()), orders(new OrdersList()), territories() {}

Player::Player(const Player &original) {
    playerName = original.playerName;
    territories = vector<Territory *>(original.territories.size());
    for (int i = 0; i < territories.size(); i++) {
        territories[i] = new Territory(*original.territories[i]);
    }

    handOfCards = new Hand(*original.handOfCards);
    orders = new OrdersList(*original.orders);
}

Player &Player::operator=(const Player &otherPlayer) {
    playerName = otherPlayer.playerName;
    territories = vector<Territory *>(otherPlayer.territories.size());
    for (int i = 0; i < territories.size(); i++) {
        territories[i] = new Territory(*otherPlayer.territories[i]);
    }

    handOfCards = new Hand(*otherPlayer.handOfCards);
    orders = new OrdersList(*otherPlayer.orders);

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Player &player) {
    return stream << "Information on Player object:" << endl
                  << "Address: " << &player << endl
                  << "Player Name: " << player.getPlayerName() << endl
                  << "Number of Territories Owned: " << player.getTerritories().size() << endl
                  << "Size of Hand: " << player.getHandofCards()->getCards().size() << endl
                  << "Number of Orders: " << player.getOrders()->getOrderList().size() << endl;
}

vector<int> Player::toDefend() {
    vector<int> territoriesToDefend;
    for(auto territory: territories) {
        territoriesToDefend.push_back(territory->getTerritoryId());
    }

    return territoriesToDefend;
}

vector<int> Player::toAttack() {
    vector<int> territoriesToAttack;

    vector<int> territoriesOwned;
    for(auto territory: territories) {
        territoriesOwned.push_back(territory->getTerritoryId());
    }

    for(auto territory: territories) {
        for(auto adjTerritoryId: territory->getAdjList()) {
            if(find(territoriesOwned.begin(), territoriesOwned.end(), adjTerritoryId) == territoriesOwned.end() &&
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjTerritoryId) == territoriesToAttack.end())
                territoriesToAttack.push_back(adjTerritoryId);
        }
    }

    return territoriesToAttack;
}

void Player::issueOrder() {
    DeployOrder* deployOrder = new DeployOrder();
    AdvanceOrder* advanceOrder = new AdvanceOrder();

    orders->add(deployOrder);
    cout << "Deploy order added to the list for " << this->getPlayerName() << endl;
    orders->add(advanceOrder);
    cout << "Advance order added to the list for " << this->getPlayerName() << endl;

    cout << endl;
}

// Getters
string Player::getPlayerName() {
    return this->playerName;
}

vector<Territory *>& Player::getTerritories() {
    return this->territories;
}

Hand* Player::getHandofCards() {
    return this->handOfCards;
}

OrdersList* Player::getOrders() {
    return this->orders;
}

// Setters
void Player::setPlayerName(string playerName) {
    this->playerName = playerName;
}

void Player::setTerritories(vector<Territory *> &territories) {
    this->territories = territories;
}

void Player::setHandOfCards(Hand *handOfCards) {
    this->handOfCards = handOfCards;
}

void Player::setOrders(OrdersList* orders) {
    this->orders = orders;
}

Player::~Player()
{
    delete handOfCards;
    handOfCards = nullptr;
    delete orders;
    orders = nullptr;
}
