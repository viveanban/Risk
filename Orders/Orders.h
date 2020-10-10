#ifndef RISK_ORDERS_H
#define RISK_ORDERS_H

#include <vector>
#include <iostream>

using namespace std;

/**
 * This class represents an Order that a player can give for the Risk Game
 * The possible types of Orders are Deploy, Advance, Bomb, Blockade, Airlift, Negotiate.
 */
class Order {
public:
    string description;

    friend ostream &operator<<(ostream &stream, Order &order);

    // The following methods are pure virtual functions (must be overridden)
    /**
     * Checks if the Order given is a valid order.
     * @return boolean
     */
    virtual bool validate() = 0;

    /**
     * Executes the Order's actions.
     */
    virtual void execute() = 0;

    virtual ~Order();
};

/**
 * Subclasses of Order
 */
class DeployOrder : public Order {
public:
    DeployOrder();

    DeployOrder(const DeployOrder &original);

    DeployOrder &operator=(const DeployOrder &order) ;

private:
    bool validate() override;

    void execute() override;
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();

    AdvanceOrder(const AdvanceOrder &original);

    AdvanceOrder &operator=(const AdvanceOrder &order);
private:
    bool validate() override;

    void execute() override;
};

class BombOrder : public Order {
public:
    BombOrder();

    BombOrder(const BombOrder &original);

    BombOrder &operator=(const BombOrder &order);


private:

    bool validate() override;

    void execute() override;
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();

    BlockadeOrder(const BlockadeOrder &original);

    BlockadeOrder &operator=(const BlockadeOrder &order) ;
private:
    bool validate() override;

    void execute() override;

};

class AirliftOrder : public Order {
public:
    AirliftOrder();

    AirliftOrder(const AirliftOrder &original);

    AirliftOrder &operator=(const AirliftOrder &order);
private:
    bool validate() override;

    void execute() override;

};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();

    NegotiateOrder(const NegotiateOrder &original);

    NegotiateOrder &operator=(const NegotiateOrder &order);

private:
    bool validate() override;

    void execute() override;
};

class ReinforcementOrder : public Order {
public:
    ReinforcementOrder();

    ReinforcementOrder(const ReinforcementOrder &original);

    ReinforcementOrder &operator=(const ReinforcementOrder &order) ;

private:
    bool validate() override;

    void execute() override;
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

    void executeAll();

    vector<Order *> &getOrderList();
};

#endif //RISK_ORDERS_H
