#include <iostream>
#include "Player.h"
#include <algorithm>
#include <set>

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
        issueDeployOrder();
        return true;
    } else { // Other orders
        bool continueIssuingOrders = rand() % 2;

        if (continueIssuingOrders) {
            // Choose advance or hand?
            bool advance = rand() % 2;
            if (advance) {
                issueAdvanceOrder();
            }
            else
            {
                Card* card = handOfCards->getRandomCard();

                switch(card->getType()) {
                    case Card::CardType::diplomacy:
                        break;

                    case Card::CardType::reinforcement:
                        break;

                    case Card::CardType::blockade:
                        issueBlockadeOrder();
                        break;

                    case Card::CardType::bomb:
                        break;

                    case Card::CardType::airlift:
                        issueAirliftOrder();
                        break;

                    default:
                        break;
                }
            }
        }

        return continueIssuingOrders;
    }
}

// TODO: move all theses to Order and have a virtual issue() method that each class implements. Also, send the Player object.
void Player::issueDeployOrder() {
    // This ensures that the numberOfArmiesToDeploy is always smaller or equal than numberOfArmies
    int numberOfArmiesToDeploy = (rand() % numberOfArmies) + 1;

    Territory *targetTerritory = territories.at(rand() % territories.size());

    // Update number of armies
    numberOfArmies -= numberOfArmiesToDeploy;

    // Update order list
    orders->add(new DeployOrder(targetTerritory, numberOfArmiesToDeploy));
}

void Player::issueAdvanceOrder() {
    // Determine src territory
    Territory *srcTerritory = territories.at(rand() % territories.size());

    // Determine target territory
    bool attack = rand() % 2;
    vector<Territory*> territoriesToChooseFrom = attack ? toAttack() : toDefend();
    Territory *targetTerritory = territoriesToChooseFrom.at(rand() % territoriesToChooseFrom.size());

    // Determine number of armies to advance
    int numberOfArmiesToAdvance = (rand() % srcTerritory->getUnitNbr()) + 1;

    // Update order list
    orders->add(new AdvanceOrder(srcTerritory, targetTerritory, numberOfArmiesToAdvance));
}

// TODO: implement
void Player::issueBombOrder() {
    Territory *targetTerritory;
}

void Player::issueAirliftOrder() {
    // Determine src territory
    Territory *srcTerritory = territories.at(rand() % territories.size());

    // Determine target territory
    Territory *targetTerritory = territories.at(rand() % territories.size());

    // Determine number of armies to advance
    int numberOfArmiesToAirlift = (rand() % srcTerritory->getUnitNbr()) + 1;

    // Update order list
    orders->add(new AirliftOrder(srcTerritory, targetTerritory, numberOfArmiesToAirlift));
}

void Player::issueBlockadeOrder() {
    // Determine target territory
    Territory *targetTerritory = territories.at(rand() % territories.size());

    // Update order list
    orders->add(new BlockadeOrder(targetTerritory));
}

// TODO: implement
void Player::issueReinforcementOrder() {

}

// TODO: implement
void Player::issueNegotiateOrder() {
    // Target player

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

