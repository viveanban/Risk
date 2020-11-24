#ifndef RISK_PLAYERSTRATEGIES_H
#define RISK_PLAYERSTRATEGIES_H

#include "../Player/Player.h"

class PlayerStrategy {
private:
    Player* player;
public:
    virtual bool issueOrder() = 0;

    virtual vector<Territory *> toAttack() = 0;

    virtual vector<Territory *> toDefend() = 0;
};

class HumanPlayerStrategy : PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;
};

class AggressivePlayerStrategy : PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;
};

class BenevolentPlayerStrategy : PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;
};

class NeutralPlayerStrategy : PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;
};

#endif //RISK_PLAYERSTRATEGIES_H
