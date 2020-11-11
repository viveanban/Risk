#include <iostream>
#include "Player.h"
#include <algorithm>
#include <set>
#include "./../Orders/Orders.h"

/**
 * Player Class implementation
 */
Player::Player() : playerName(), handOfCards(new Hand()), orders(new OrdersList()), territories() {}

Player::Player(string playerName) : playerName(playerName), handOfCards(new Hand()), orders(new OrdersList()), territories() {}

Player::~Player() {
    delete handOfCards;
    handOfCards = nullptr;
    delete orders;
    orders = nullptr;
}

Player::Player(const Player &original) {
    playerName = original.playerName;
    territories = vector<Territory *>(original.territories.size());
    for (int i = 0; i < territories.size(); i++) {
        territories[i] = original.territories[i];
    }

    handOfCards = new Hand(*original.handOfCards);
    orders = new OrdersList(*original.orders);
}

Player &Player::operator=(const Player &otherPlayer) {
    playerName = otherPlayer.playerName;
    territories = vector<Territory *>(otherPlayer.territories.size());
    for (int i = 0; i < territories.size(); i++) {
        territories[i] = otherPlayer.territories[i];
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
                  << "Number of Orders: " << player.getOrders()->getOrderList().size() << endl
                  << "Number of Armies: " << player.numberOfArmies << endl;
}

//TODO: Change method to take a srcTerritory as param
vector<Territory *> Player::toDefend() {
    return territories;
}

//TODO: Change method to take a srcTerritory as param
vector<Territory *> Player::toAttack() {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: territories) {
        for (Territory *adjTerritory: territory->getAdjList()) {
            if (find(territories.begin(), territories.end(), adjTerritory) == territories.end() &&
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjTerritory) == territoriesToAttack.end())
                territoriesToAttack.push_back(adjTerritory);
        }
    }

    return territoriesToAttack;
}

bool Player::issueOrder() {
    if (this->getNumberofArmies() > 0) { // Deploy orders
        (new DeployOrder())->issue(this);
        return true;
    } else { // Other orders
        bool continueIssuingOrders = rand() % 2;

        if (continueIssuingOrders) {
            // Choose advance or hand?
            bool advance = rand() % 2;
            if (advance) {
                (new AdvanceOrder())->issue(this);
            }
            else
            {
                Card* card = handOfCards->getRandomCard();
                Order* order = card->play();
                order->issue(this);
                orders->add(order);
                handOfCards->removeCard(card);
            }
        }

        return continueIssuingOrders;
    }
}

// Getters
string Player::getPlayerName() {
    return this->playerName;
}

vector<Territory *> &Player::getTerritories() {
    return this->territories;
}

Hand *Player::getHandofCards() {
    return this->handOfCards;
}

OrdersList *Player::getOrders() {
    return this->orders;
}

int Player::getNumberofArmies() {
    return this->numberOfArmies;
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

void Player::setOrders(OrdersList *orders) {
    this->orders = orders;
}

void Player::setNumberOfArmies(int numberOfArmies) {
    this->numberOfArmies = numberOfArmies;
}

