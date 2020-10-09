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
//    for (int i = 0; i < territories.size(); i++) {
//        territories[i] = new Territory(*original.territories[i]);
//    }

    handOfCards = original.handOfCards;
    orders = original.orders;
}

Player &Player::operator=(const Player &otherPlayer) {
    playerName = otherPlayer.playerName;
    territories = vector<Territory *>(otherPlayer.territories.size());
//    for (int i = 0; i < territories.size(); i++) {
//        territories[i] = new Territory(*otherPlayer.territories[i]);
//    }

    handOfCards = otherPlayer.handOfCards;
    orders = otherPlayer.orders;

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

vector<Territory *> Player::toDefend() {
    return territories;
}

vector<Territory *> Player::toAttack() {
    vector<Territory *> territoriesToAttack;
    for(auto territory: territories) {
        for(auto adjTerritory: territory->getAdjList()) {
            if(find(territories.begin(), territories.end(), adjTerritory) == territories.end() &&
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjTerritory) == territoriesToAttack.end())
                territoriesToAttack.push_back(adjTerritory);
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
    handOfCards = NULL;
    delete orders;
    orders = NULL;
}
