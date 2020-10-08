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
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*original.territories[i]);

    handOfCards = original.handOfCards;
    orders = original.orders;
}

Player &Player::operator=(const Player &otherPlayer) {
    playerName = otherPlayer.playerName;
    territories = vector<Territory *>(otherPlayer.territories.size());
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*otherPlayer.territories[i]);

    handOfCards = otherPlayer.handOfCards;
    orders = otherPlayer.orders;

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Player &player) {
    return stream << "\tInformation on Player object:" << endl
                  << "\tPlayer Name: " << player.getPlayerName() << endl
                  << "\tNumber of Territories Owned: " << player.getTerritories().size()
                  << "\tSize of Hand: " << player.getHandofCards()->getCards().size()
                  << "\tNumber of Orders: " << player.getOrders()->getOrderList().size() << endl;
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
    // Create Order objects
    DeployOrder* deployOrder = new DeployOrder();
    AdvanceOrder* advanceOrder = new AdvanceOrder();
    orders->add(deployOrder);
    orders->add(advanceOrder);

//    for(Card* card: handOfCards->getCards()) {
//        Order* order = card->play();
//        orders->add(order);
//    }

    cout << "Orders created and added to list of orders for " << this->getPlayerName() << endl;
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
