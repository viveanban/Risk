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

    // ----------Deploy Order methods--------------

    /**
     * Issue a deploy order and potentially a reinforcement card.
     */
    virtual void issueDeployOrder();

    virtual bool setUpDeployOrder(DeployOrder* order);

    /**
     * Randomly decide if the player plays his reinforcement card, if yes then add +5 unit armies to the player.
     */
    virtual void playReinforcementCard();

    // ----------Deploy Order methods--------------

    // ----------Advance Order methods--------------

    /**
     * Issue an advance order, randomly decides whether to attack or defend and select a random territory to do so.
     */
    virtual void issueAdvanceOrder();

    virtual bool setUpAdvanceOrder(AdvanceOrder* order);

    // ----------Advance Order methods--------------

    // ----------Other Order methods--------------

    /**
     * Issue any non-deploy and non-advance order triggered by a card.
     * @param cardChosen : card to play
     */
    virtual void issueOrderFromCard(Card* cardChosen);

    virtual bool issueBombOrder(BombOrder* order);

    virtual bool issueBlockadeOrder(BlockadeOrder* order);

    virtual bool issueAirliftOrder(AirliftOrder* order);

    virtual bool issueNegotiateOrder(NegotiateOrder* order);

    // ----------Advance Order methods--------------

    virtual ~PlayerStrategy();

    static bool getBooleanInput(string printStatement);

    static int getIntegerInput(string printStatement, int leftBound, int rightBound);
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

    bool setUpDeployOrder(DeployOrder *order) override;

    void playReinforcementCard() override;

    bool setUpAdvanceOrder(AdvanceOrder* order) override;

    bool issueBombOrder(BombOrder* order) override;

    bool issueBlockadeOrder(BlockadeOrder* order) override;

    bool issueAirliftOrder(AirliftOrder* order) override;

    bool issueNegotiateOrder(NegotiateOrder* order) override;
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
