#ifndef RISK_PLAYERSTRATEGIES_H
#define RISK_PLAYERSTRATEGIES_H

#include "../Player/Player.h"

// TODO: correct copy constructor, assignment operator, and stream insertion operator. (Abhijit)
class PlayerStrategy {
public:
    Player *player;

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

    virtual bool issueBlockadeOrder(BlockadeOrder *order);

    virtual bool issueAirliftOrder(AirliftOrder *order);

    virtual bool issueNegotiateOrder(NegotiateOrder *order);

    // ----------Advance Order methods--------------

    virtual ~PlayerStrategy();

    PlayerStrategy();

    PlayerStrategy(Player *player);

    static bool getBooleanInput(string printStatement);

    static int getIntegerInput(string printStatement, int leftBound, int rightBound);

    PlayerStrategy(const PlayerStrategy &original);

    PlayerStrategy &operator=(const PlayerStrategy &other);

    friend std::ostream &operator<<(ostream &stream, PlayerStrategy &playerStrategy);
};

/*
 * Human player that requires user interaction to make decisions.
 */
class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();

    explicit HumanPlayerStrategy(Player *player);

    HumanPlayerStrategy(const HumanPlayerStrategy &original);

    HumanPlayerStrategy &operator=(const HumanPlayerStrategy &other);

    friend std::ostream &operator<<(ostream &stream, HumanPlayerStrategy &playerStrategy);

    virtual ~HumanPlayerStrategy();

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
    AggressivePlayerStrategy();

    explicit AggressivePlayerStrategy(Player *player);

    AggressivePlayerStrategy(const AggressivePlayerStrategy &original);

    AggressivePlayerStrategy &operator=(const AggressivePlayerStrategy &other);

    friend std::ostream &operator<<(ostream &stream, AggressivePlayerStrategy &playerStrategy);

    virtual ~AggressivePlayerStrategy();

    bool issueOrder() override;

    bool setUpAdvanceOrder(AdvanceOrder *order) override;

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
    BenevolentPlayerStrategy();

    explicit BenevolentPlayerStrategy(Player *player);

    BenevolentPlayerStrategy(const BenevolentPlayerStrategy &original);

    BenevolentPlayerStrategy &operator=(const BenevolentPlayerStrategy &other);

    friend std::ostream &operator<<(ostream &stream, BenevolentPlayerStrategy &playerStrategy);

    virtual ~BenevolentPlayerStrategy();

    bool issueOrder() override;

    bool setUpAdvanceOrder(AdvanceOrder *order) override;

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
    NeutralPlayerStrategy();

    explicit NeutralPlayerStrategy(Player *player);

    NeutralPlayerStrategy(const NeutralPlayerStrategy &original);

    NeutralPlayerStrategy &operator=(const NeutralPlayerStrategy &other);

    friend std::ostream &operator<<(ostream &stream, NeutralPlayerStrategy &playerStrategy);

    virtual ~NeutralPlayerStrategy();

    bool issueOrder() override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};

/*
 * A random player that issues can both defend and attack using the most appropriate territories.
 */
class RandomPlayerStrategy : public PlayerStrategy {
public:
    explicit RandomPlayerStrategy(Player *player);

    bool issueOrder() override;

    bool setUpDeployOrder(DeployOrder *order) override;

    bool setUpAdvanceOrder(AdvanceOrder *order) override;

    vector<Territory *> toAttack() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack(Territory *srcTerritory) override;

    vector<Territory *> toDefend(Territory *srcTerritory) override;
};
#endif //RISK_PLAYERSTRATEGIES_H
