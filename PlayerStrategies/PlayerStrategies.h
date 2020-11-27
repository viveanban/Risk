#ifndef RISK_PLAYERSTRATEGIES_H
#define RISK_PLAYERSTRATEGIES_H

#include "../Player/Player.h"

class PlayerStrategy {
protected:
    Player *player;
public:
    virtual bool issueOrder() = 0;

    virtual vector<Territory *> toAttack() = 0;

    virtual vector<Territory *> toAttack(Territory *srcTerritory) = 0;

    virtual vector<Territory *> toDefend() = 0;

    virtual vector<Territory *> toDefend(Territory *srcTerritory) = 0;
};

/*
 * Human player that requires user interaction to make decisions.
 */
class HumanPlayerStrategy : PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player *player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

/*
 * An aggressive computer player focuses
 * on attack (reinforces its strongest country, then always attack with it until it cannot attack anymore,
 * then fortifies in order to maximize aggregation of forces in one country.
 */
class AggressivePlayerStrategy : PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player *player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

/*
 * A benevolent computer player that focuses on protecting its weak countries
 * (reinforces its weakest countries, never attacks,
 * then fortifies in order to move armies to weaker countries.
 */
class BenevolentPlayerStrategy : PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player *player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

/*
 * A neutral player that never issues any order.
 */
class NeutralPlayerStrategy : PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player *player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

#endif //RISK_PLAYERSTRATEGIES_H
