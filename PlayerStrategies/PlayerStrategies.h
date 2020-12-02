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

    virtual ~PlayerStrategy();
};

/*
 * Human player that requires user interaction to make decisions.
 */
class HumanPlayerStrategy : public PlayerStrategy {
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
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player *player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

/*
 * A benevolent computer player focuses on protecting its weak countries
 * (reinforces its weakest countries, never attacks,
 * then fortifies in order to move armies to weaker countries.
 */
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player *player);

    bool issueOrder() override;

    /**
     * toAttack takes care of returning a list of territories that the player can attack
     * This method will never be called however since the player won't ever attack another territory
     * @return a list of territories that the player can attack
     */
    vector<Territory *> toAttack() override;

    /**
     * toDefend takes care of returning a list of territories that the player can defend
     * the territory with least number of unit deployed in it will be first.
     * @return a list of territories that the player can defend
     */
    vector<Territory *> toDefend() override;

    /**
     * toAttack takes care of returning a list of territories that the player can attack based on a source territory
     * This method will never be called however since the player won't ever attack another territory
     * @param srcTerritory
     * @return a list of territories that the player can attack
     */
    vector<Territory *> toAttack(Territory *srcTerritory) override;

    /**
     * toDefend takes care of returning a list of territories that the player can defend from a source territory
     * the territory with least number of unit deployed in it will be first.
     * @param srcTerritory
     * @return a list of territories that the player can defend
     */
    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

/*
 * A neutral player that never issues any order.
 */
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player *player);

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

#endif //RISK_PLAYERSTRATEGIES_H
