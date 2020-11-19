#include <iostream>
#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <algorithm>

/**
 * Player Class implementation
 */

Player *Player::neutralPlayer = new Player("Neutral Player");

Player::Player(string playerName) : playerName(playerName), handOfCards(new Hand()), orders(new OrdersList()),
                                    numberOfArmiesInReinforcementPool(0), territories() {}

Player::~Player() {
    delete handOfCards;
    handOfCards = nullptr;
    cout << "Deleted Hand" << endl;

    delete orders;
    orders = nullptr;
    cout << "Deleted orders" << endl;
}

Player::Player(const Player &original) {
    playerName = original.playerName;
    territories = vector<Territory *>(original.territories.size());
    for (int i = 0; i < territories.size(); i++) {
        territories[i] = original.territories[i];
    }
    numberOfArmies = original.numberOfArmies;
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
        numberOfArmies = otherPlayer.numberOfArmies;

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
    sortTerritoryList(territories);
    return territories;
}

vector<Territory *> Player::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    sortTerritoryList(territoriesToDefend);
    return territoriesToDefend;
}

vector<Territory *> Player::toAttack() {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this)
            territoriesToAttack.push_back(territory);
    }

    sortTerritoryList(territoriesToAttack);

    return territoriesToAttack;
}

vector<Territory *> Player::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this)
            territoriesToAttack.push_back(territory);
    }

    sortTerritoryList(territoriesToAttack);

    return territoriesToAttack;
}

void Player::sortTerritoryList(vector<Territory *> &territoryList) {
    sort(territoryList.begin(), territoryList.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
}

bool Player::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (numberOfArmiesInReinforcementPool > 0) {
        issueDeployOrder();
        return true;
    } else { // Other orders
        bool continueIssuingOrders = rand() % 2;
        if (continueIssuingOrders) {
            bool advance = handOfCards->getCards().empty() || rand() % 2;
            if (advance) { //Always issue an Advance order if player has an empty hand
                issueAdvanceOrder();
            } else {
                // Pick a card
                Card *cardChosen = handOfCards->getNextCard();
                if (!cardChosen) return continueIssuingOrders; // if the reinforcement card was picked, just continue...

                // Play card
                issueOrderFromCard(cardChosen);
            }
        }
        return continueIssuingOrders;
    }
}

void Player::issueDeployOrder() {
    //Reinforcement card
    playReinforcementCard();

    // Deploy order
    Order *deployOrder = new DeployOrder(this);
    bool successful = deployOrder->issue();
    if (!successful) {
        delete deployOrder;
        deployOrder = nullptr;
    } else {
        GameEngine::getInstance()->getGameState()->updateGameState(this, issuing_orders, deployOrder, nullptr);
    }
}

void Player::playReinforcementCard() {
    for (Card *card: handOfCards->getCards()) {
        if (card->getType() == Card::reinforcement) {
            bool playReinforcementCard = rand() % 2;
            if (playReinforcementCard) {
                numberOfArmiesInReinforcementPool += numberOfArmiesInReinforcementPool + 5;
                handOfCards->removeCard(card);
                GameEngine::getInstance()->getGameState()->updateGameState(this, issuing_orders, nullptr, card);
            }
            break;
        }
    }
}

void Player::issueAdvanceOrder() {
    auto *advanceOrder = new AdvanceOrder(this);
    bool successful = advanceOrder->issue();
    if (!successful) {
        delete advanceOrder;
        advanceOrder = nullptr;
    } else {
        GameEngine::getInstance()->getGameState()->updateGameState(this, issuing_orders, advanceOrder, nullptr);
    }
}

void Player::issueOrderFromCard(Card *cardChosen) {
    Order *order = cardChosen->play();
    if (order) {
        order->setPlayer(this);
        bool successful = order->issue();
        if (!successful) {
            delete order;
            order = nullptr;
        } else {
            GameEngine::getInstance()->getGameState()->updateGameState(this, issuing_orders, order, cardChosen);
            handOfCards->removeCard(cardChosen);
        }
    }
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
