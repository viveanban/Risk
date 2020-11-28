#include "PlayerStrategies.h"

// HUMAN PLAYER STRATEGY
HumanPlayerStrategy::HumanPlayerStrategy(Player *player) {
    this->player = player;
}

bool HumanPlayerStrategy::issueOrder() {
    while (this->player->getNumberofArmiesInReinforcementPool() > 0) {
        cout << "Please issue a Deploy Order." << endl;
        this->player->issueDeployOrder();
        return true;
    }
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (this->player->getNumberofArmiesInReinforcementPool() > 0) {


    } else { // Other orders
        cout << "Please select the type of order you wish to issue: " << endl;
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
    return false;
}

vector<Territory *> HumanPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> HumanPlayerStrategy::toDefend() {
    return vector<Territory *>();
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
