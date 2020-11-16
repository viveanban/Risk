#include <iostream>
#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <algorithm>

/**
 * Player Class implementation
 */

Player *Player::neutralPlayer = new Player("Neutral Player");

Player::Player(string playerName) : playerName(playerName), handOfCards(new Hand()), orders(new OrdersList()),
                                    territories() {}

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
                  << "Size of Hand: " << player.getHandOfCards()->getCards().size() << endl
                  << "Number of Orders: " << player.getOrders()->getOrderList().size() << endl
                  << "Number of Armies: " << player.numberOfArmies << endl;
}

//TODO: Add comment that you cannot call the setOwner method from here, sinon it'll be in an endless loop
void Player::addTerritory(Territory *territory) {
    territories.push_back(territory);
}

//TODO: Add comment that you cannot call the setOwner method from here, sinon it'll be in an endless loop
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

// TODO: sprinkle move/remove()
bool Player::issueOrder() {

    if (numberOfArmies > 0) {
        // Reinforcement card
        for (Card *card: handOfCards->getCards()) {
            if (card->getType() == Card::CardType::reinforcement) {
                bool playReinforcementCard = rand() % 2;
                if (playReinforcementCard) {
                    numberOfArmies += numberOfArmies + 5;
                    handOfCards->removeCard(card);
                    if (GameEngine::getInstance()->isPhaseObserverActive())
                        cout << getPlayerName()
                             << " played a reinforcement card and got +5 armies added to his army pool"
                             << endl;
                }
                break;
            }
        }

        // Deploy order
        (new DeployOrder(this))->issue();

        return true;
    } else { // Other orders
        bool continueIssuingOrders = rand() % 2;

        if (continueIssuingOrders) {
            bool advance = rand() % 2;
            if (advance) {
                (new AdvanceOrder(this))->issue();
            } else {
                // Pick a card
                Card *cardChosen = handOfCards->getNextCard();
                if (!cardChosen) return continueIssuingOrders; // if the reinforcement card was picked, just continue...

                // Play card
                Order *order = cardChosen->play();
                if (order) {
                    order->issue();
                    orders->add(order);
                    handOfCards->removeCard(cardChosen);
                }
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

Hand *Player::getHandOfCards() {
    return this->handOfCards;
}

OrdersList *Player::getOrders() {
    return this->orders;
}

int Player::getNumberofArmies() {
    return this->numberOfArmies;
}

vector<Player *> &Player::getPlayersNotToAttack() {
    return playersNotToAttack;
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

void Player::setPlayersNotToAttack(const vector<Player *> &playersNotToAttack) {
    Player::playersNotToAttack = playersNotToAttack;
}
