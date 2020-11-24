#include "PlayerStrategies.h"

// HUMAN PLAYER STRATEGY
HumanPlayerStrategy::HumanPlayerStrategy(Player *player) {
    this->player = player;
}

bool HumanPlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> HumanPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> HumanPlayerStrategy::toDefend() {
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
