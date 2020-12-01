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
    // don't need to sort for human player because they can choose any from list.
    return territoriesToAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend() {
    // don't need to sort for human player because they can choose any from list.
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    // don't need to sort for human player because they can choose any from list.
    return territoriesToAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    // don't need to sort for human player because they can choose any from list.
    return territoriesToDefend;
}

// AGGRESIVE PLAYER STRATEGY
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) {
    this->player = player;
}

bool AggressivePlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> AggressivePlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toDefend() {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
}

// BENEVOLENT PLAYER STRATEGY
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) {
    this->player = player;
}

bool BenevolentPlayerStrategy::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (this->player->getNumberofArmiesInReinforcementPool() > 0) {
        this->player->issueDeployOrder();
        return true;
    } else {
        //Since benevolent player cannot conquer a territory, he won't have a card, he can just move to weaker territories.
        bool continueIssuingOrders = rand() % 2;
        if (continueIssuingOrders) {
            // sort his territories from strongest to weakest so he picks strongest territory as source territory
            std::sort(player->getTerritories().begin(),
                      player->getTerritories().end(),
                      [](Territory &territory1, Territory &territory2) {
                          return territory1.getUnitNbr() > territory2.getUnitNbr();
                      });
            this->player->issueAdvanceOrder();
        }
        return continueIssuingOrders;
    }
}

vector<Territory *> BenevolentPlayerStrategy::toAttack() {
    //It is never going to call this method, but we leave it since it wouldn't be correct to not have an attack method
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    return territoriesToAttack;
}

vector<Territory *> BenevolentPlayerStrategy::toDefend() {
    // Need to return territories in order of weakest --> strongest
    std::sort(player->getTerritories().begin(),
              player->getTerritories().end(),
              [](Territory &territory1, Territory &territory2) {
                  return territory1.getUnitNbr() < territory2.getUnitNbr();
              });
    return player->getTerritories();
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Territory *srcTerritory) {

    vector<Territory *> territoriesToAttack;
    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    return territoriesToAttack;
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    //sort from weakest to strongest
    std::sort(territoriesToDefend.begin(),
              territoriesToDefend.end(),
              [](Territory &territory1, Territory &territory2) {
                  return territory1.getUnitNbr() < territory2.getUnitNbr();
              });
    return territoriesToDefend;
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
