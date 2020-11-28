#include "PlayerStrategies.h"

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
                vector<Card *> handOfCards = player->getHandOfCards()->getCards();
                for (int i = 0; i < handOfCards.size(); ++i) {
                    cout << i << " - " << handOfCards.at(i)->getTypeName() << endl;
                }
                // Pick a card
                Card *cardChosen = handOfCards.at(Player::getIntegerInput("Please select the card you wish to play: "));
                // Play card
                player->issueOrderFromCard(cardChosen);
            }
        }
        return continueIssuingOrders;
    }
}

vector<Territory *> HumanPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> HumanPlayerStrategy::toDefend() {
    player->sortTerritoryList(player->getTerritories());
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> HumanPlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
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
