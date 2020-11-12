#include <iostream>
#include "Player.h"
#include <algorithm>

/**
 * Player Class implementation
 */

Player* Player::neutralPlayer = new Player("Neutral Player");

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

// TODO: toDefend(): returns all territories owned by player but it is prioritized by number of armit units that territory has

// TODO: toDefend(srcTerritory): returns all adjacent territories from srcTerritory that it can defend + prioritized by number of armit units

// TODO: toAttack(): returns all enemy territories + prioritized by number of armit units

// TODO: toAttack(srcTerritory): returns all adjacent territories from srcTerritory that it can attack + prioritized by number of armit units



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

// TODO: sprinkle move/remove()
bool Player::issueOrder() {

    if (numberOfArmies > 0) {
        // Reinforcement card
        for (Card *card: handOfCards->getCards()) {
            if (card->getType() == Card::CardType::reinforcement) {
                bool playReinforcementCard = rand() % 2;
                if(playReinforcementCard) {
                    numberOfArmies += numberOfArmies + 5;
                }
                break; // TODO: check if breaks from for loop
            }
        }

        // Deploy order
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
                Card* cardChosen = nullptr;
                for(Card* card: handOfCards->getCards()) {
                    if(card->getType() != Card::CardType::reinforcement)
                        cardChosen = card;
                }

                if(!cardChosen) return continueIssuingOrders;

                Order* order = cardChosen->play();
                order->issue(this);
                orders->add(order);
                handOfCards->removeCard(cardChosen);
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

const vector<Player *> &Player::getPlayersNotToAttack() const {
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

