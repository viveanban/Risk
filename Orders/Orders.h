#ifndef RISK_ORDERS_H
#define RISK_ORDERS_H

#include <vector>
#include <iostream>
#include "../Map/Map.h"

using namespace std;

/**
 * This class represents an Order that a player can give for the Risk Game
 * The possible types of Orders are Deploy, Advance, Bomb, Blockade, Airlift, Negotiate.
 */
class Order {
private:
    string name;

    int priority;

protected:
    string description;

    Player* player;

public:
    Order(string name, int priority, Player * player);

    Order(const Order &original);

    Order &operator=(const Order &otherOrder);

    friend ostream &operator<<(ostream &stream, Order &order);

    /**
     * Checks if the Order given is a valid order.
     * @return boolean
     */
    virtual bool validate() = 0;

    /**
     * Executes the Order's actions.
     */
    virtual void execute() = 0;

    virtual bool issue() = 0;

    const string &getName() const;

    const string &getDescription() const;

    int getPriority() const;

    void setPlayer(Player *player);
};

/**
 * Places some armies on one of the current player’s territories.
 */
class DeployOrder : public Order {
private:
    Territory *targetTerritory;

    int numberOfArmiesToDeploy;

    bool validate() override;

public:
    DeployOrder();

    explicit DeployOrder(Player * player);

    DeployOrder(const DeployOrder &original);

    DeployOrder &operator=(const DeployOrder &otherOrder);

    void execute() override;

    bool issue() override;

    Territory *getTargetTerritory() const;

    int getNumberOfArmiesToDeploy() const;
};

enum AdvanceOrderType { attack, transfer };

/**
 * Move some armies from one of the current player’s territories (source) to an adjacent territory
 * (target). If the target territory belongs to the current player, the armies are moved to the target
 * territory. If the target territory belongs to another player, an attack happens between the two
 * territories.
 */
class AdvanceOrder : public Order {
private:
    Territory *sourceTerritory;

    Territory *targetTerritory;

    AdvanceOrderType advanceOrderType;

    int numberOfArmiesToAdvance;

    bool validate() override;

    bool kill(int probabilityToKill);

public:
    AdvanceOrder();

    explicit AdvanceOrder(Player * player);

    AdvanceOrder(const AdvanceOrder &original);

    AdvanceOrder &operator=(const AdvanceOrder &otherOrder);

    void execute() override;

    bool issue() override;

    Territory *getSourceTerritory() const;

    Territory *getTargetTerritory() const;

    int getNumberOfArmiesToAdvance() const;

    AdvanceOrderType getAdvanceOrderType() const;
};

/**
 * Destroy half of the armies located on an opponent’s territory that is adjacent to one of the current
 * player’s territories.
 */
class BombOrder : public Order {
private:
    Territory* targetTerritory;

    bool validate() override;

public:
    BombOrder();

    explicit BombOrder(Player * player);

    BombOrder(const BombOrder &original);

    BombOrder &operator=(const BombOrder &otherOrder);

    void execute() override;

    bool issue() override;

    Territory *getTargetTerritory() const;
};

/**
 * Triple the number of armies on one of the current player’s territories and make it a neutral territory
 */
class BlockadeOrder : public Order {
private:
    Territory *targetTerritory;

    bool validate() override;

public:
    BlockadeOrder();

    explicit BlockadeOrder(Player * player);

    BlockadeOrder(const BlockadeOrder &original);

    BlockadeOrder &operator=(const BlockadeOrder &otherOrder);

    void execute() override;

    bool issue() override;

    Territory *getTargetTerritory() const;
};

/**
 * Advance some armies from one of the current player’s territories to any another territory.
 */
class AirliftOrder : public Order {
private:
    Territory *sourceTerritory;

    Territory *targetTerritory;

    int numberOfArmiesToAirlift;

    bool validate() override;

public:
    AirliftOrder();

    explicit AirliftOrder(Player * player);

    AirliftOrder(const AirliftOrder &original);

    AirliftOrder &operator=(const AirliftOrder &otherOrder);

    void execute() override;

    bool issue() override;

    Territory *getSourceTerritory() const;

    Territory *getTargetTerritory() const;

    int getNumberOfArmiesToAirlift() const;
};

/**
 * Prevent attacks between the current player and another player until the end of the turn.
 */
class NegotiateOrder : public Order {
private:
    Player* targetPlayer;

    bool validate() override;

public:
    NegotiateOrder();

    explicit NegotiateOrder(Player * player);

    NegotiateOrder(const NegotiateOrder &original);

    NegotiateOrder &operator=(const NegotiateOrder &otherOrder);

    void execute() override;

    bool issue() override;

    Player *getTargetPlayer() const;
};

/**
 * This class represents a list of orders for the Risk game
 * An OrdersList is a list of Orders that can be Deploy, Advance, Bomb, Blockade, Airlift, Negotiate, Reinforce.
 */
class OrdersList {
private:
    vector<Order *> orderList;

    /**
     * Helper method used to create a deep copy of an existing vector of Order *. Useful for the copy constructor and
     * the assignment operator.
     * @param originalVector: vector whose contents will be copied
     * @param destinationVector: vector which will be updated with the copies
     */
    static void copyOrderList(const vector<Order *> &originalVector, vector<Order *> &destinationVector);

public:
    OrdersList();

    ~OrdersList();

    OrdersList(const OrdersList &original);

    OrdersList &operator=(const OrdersList &original);

    friend ostream &operator<<(ostream &stream, OrdersList &ordersList);

    /**
     * Adds an Order to the list of Orders
     * @param order
     */
    void add(Order *order);

    /**
     * Deletes an Order from the list of Orders
     * @param order
     * @return boolean indicating order removed successfully
     */
    bool remove(Order *order);

    /**
     * Moves an order to the given destination index
     * @param order
     * @param destination
     * @return boolean indicating if move was successful
     */
    bool move(Order *order, int destination);

    vector<Order *> &getOrderList();

    void sortOrderListByPriority();
};

#endif //RISK_ORDERS_H
