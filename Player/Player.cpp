#include <iostream>
#include "Player.h"
#include <algorithm>

/**
 * Player Class implementation
 */
Player::Player() : playerName(), handOfCards(new Hand()), orders(new OrdersList()), territories() {}

Player::~Player()
{
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

vector<Territory *> Player::toDefend() {
    return territories;
}

vector<Territory *> Player::toAttack() {
    vector<Territory *> territoriesToAttack;

    for(Territory * territory: territories) {
        for(Territory * adjTerritory: territory->getAdjList()) {
            if(find(territories.begin(), territories.end(), adjTerritory) == territories.end() &&
               find(territoriesToAttack.begin(), territoriesToAttack.end(), adjTerritory) == territoriesToAttack.end())
                territoriesToAttack.push_back(adjTerritory);
        }
    }

    return territoriesToAttack;
}

// TODO: display stuff where it is relevant for the user so that they can make a good choice
// TODO: create help guide?
bool Player::issueOrder() {
    // Deploy orders
    if(this->getNumberofArmies() > 0) {
        issueDeployOrder();
        return true;
    } else {
        char yesOrNo;
        do {
            cout << "Do you want to issue an order? (Y/N)" << endl;
            cin >> yesOrNo;
        } while(yesOrNo != 'Y' && yesOrNo != 'N');

        if(yesOrNo == 'Y') {
            cout << "Which Order would you like to issue?" << endl;
            // Print Advance Order + All Cards in Hand. Numerote

        }
        else {
            return false;
        }
    }
}

void Player::issueDeployOrder() {
    string territoryName;
    Territory* targetTerritory = nullptr;
    int numberOfArmiesToDeploy = -1;

    cout << "Here is a list of territories where you can deploy your armies: " << endl;
    // TODO: print la liste avec les armies

    // Determine Territory to Deploy On
    do {
        cout << "Enter the name of the territory you would like to deploy your armies to: ";
        cin >> territoryName;

        for (Territory *t: toDefend()) {
            if (t->getTerritoryName() == territoryName)
                targetTerritory = t;
        }

    } while(!targetTerritory);

    // Determine Number of Armies
    do {
        cout << "Enter the amount of armies you want to deploy in that territory: " << endl;
        cin >> numberOfArmiesToDeploy;
    } while(numberOfArmiesToDeploy == -1 || numberOfArmiesToDeploy > numberOfArmies);


    // Update number of Armies
    numberOfArmies -= numberOfArmiesToDeploy;

    // Update order list
    orders->add(new DeployOrder(targetTerritory, numberOfArmiesToDeploy));
}

// TODO: continue
void Player::issueAdvanceOrder() {
    string territoryName;
    Territory* targetTerritory = nullptr;
    Territory* srcTerritory = nullptr;
    int numberOfArmiesToAdvance = -1;

    cout << "Here is a list of territories that you can defend: " << endl;
    // TODO: print list
    cout << "Here is a list of territories that you can attack: " << endl;
    // TODO: print list

    // Determine Territory to Advance On
    do {
        cout << "Enter the name of the territory you would like to advance on: ";
        cin >> territoryName;

        // Name Check
        for (Territory *t: toDefend()) {
            if (t->getTerritoryName() == territoryName)
                targetTerritory = t;
        }

        if(!targetTerritory) {
            for (Territory *t: toAttack()) {
                if (t->getTerritoryName() == territoryName)
                    targetTerritory = t;
            }
        }

    } while(!targetTerritory);

    // Determine Number of Armies
    do {
        cout << "Enter the amount of armies you want to advance in that territory: " << endl;
        cin >> numberOfArmiesToAdvance;
    } while(numberOfArmiesToAdvance == -1 || numberOfArmiesToAdvance > numberOfArmies);
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

void Player::setOrders(OrdersList* orders) {
    this->orders = orders;
}

void Player::setNumberOfArmies(int numberOfArmies) {
    this->numberOfArmies = numberOfArmies;
}
