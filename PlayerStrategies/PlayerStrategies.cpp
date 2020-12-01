#include <algorithm>
#include "PlayerStrategies.h"
#include "../GameEngine/GameEngine.h"

// HUMAN PLAYER STRATEGY
HumanPlayerStrategy::HumanPlayerStrategy(Player *player) {
    this->player = player;
}

bool HumanPlayerStrategy::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (this->player->getNumberofArmiesInReinforcementPool() > 0) {
        cout << "You must issue a deploy order." << endl;
        this->player->issueDeployOrder();
        return true;
    } else { // Other orders
        bool continueIssuingOrders = Player::getBooleanInput("Do you want to continue issuing orders? [true/false] ");
        if (continueIssuingOrders) {
            if (player->getHandOfCards()->getCards().empty()) {
                cout << "You must issue an Advance Order because your hand is empty!" << endl;
                player->issueAdvanceOrder();
            } else {
                // Print all cards in Hand
                vector<Card *> handOfCards = player->getHandOfCards()->getCards();
                for (int i = 0; i < handOfCards.size(); ++i) {
                    cout << i << " - " << handOfCards.at(i)->getTypeName() << endl;
                }
                // Pick a card
                Card *cardChosen = handOfCards.at(Player::getIntegerInput("Please select the card you wish to play: ",
                                                                          0, handOfCards.size()));
                // Play card
                player->issueOrderFromCard(cardChosen);
            }
        }
        return continueIssuingOrders;
    }
}

vector<Territory *> HumanPlayerStrategy::toAttack() {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    return territoriesToAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend() {
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    return territoriesToAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    return territoriesToDefend;
}

// AGGRESIVE PLAYER STRATEGY
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) {
    this->player = player;
}

bool AggressivePlayerStrategy::issueOrder() {
    sortTerritoryListMostToLeastArmies(player->getTerritories());
    //Issue deploy orders as long as player's reinforcement pool is not empty
    if (player->getNumberofArmiesInReinforcementPool() > 0) {
        player->issueDeployOrder();
        return true;
    } else { // Other orders
        bool continueIssuingOrders = rand() % 2;
        if (continueIssuingOrders) {
            bool advance = player->getHandOfCards()->getCards().empty() || rand() % 2;
            if (advance) { //Always issue an Advance order if player has an empty hand
                player->issueAdvanceOrder();
            } else {
                // Pick a card
                discardDefensiveCards(player->getHandOfCards()->getCards());
                Card *cardChosen = player->getHandOfCards()->getNextCard();
                if (!cardChosen) return continueIssuingOrders; // if the reinforcement card was picked, just continue...
                // Play card
                player->issueOrderFromCard(cardChosen);
            }
        }
        return continueIssuingOrders;
    }
}

vector<Territory *> AggressivePlayerStrategy::toAttack() {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    return territoriesToAttack;
}

vector<Territory *> AggressivePlayerStrategy::toDefend() {
    return this->player->getTerritories();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    return territoriesToAttack;
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    return territoriesToDefend;
}

void AggressivePlayerStrategy::discardDefensiveCards(vector<Card *> &cards) {
    cards.erase(remove_if(cards.begin(), cards.end(), [](const Card &card) {
        return card.getType() == Card::blockade
               or card.getType() == Card::airlift
               or card.getType() == Card::diplomacy;
    }), cards.end());
}

// BENEVOLENT PLAYER STRATEGY
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) {
    this->player = player;
}

bool BenevolentPlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> BenevolentPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend() {
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
}


// NEUTRAL PLAYER STRATEGY
NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) {
    this->player = player;
}

bool NeutralPlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> NeutralPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toDefend() {
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
}

void PlayerStrategy::sortTerritoryListMostToLeastArmies(vector<Territory *> &territoryList) {
    sort(territoryList.begin(), territoryList.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() > rhs->getPriority();
    });
}