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
};

/**
 *
 */
class DeployOrder : public Order {
public:
    DeployOrder();

private:
    bool validate() override;

    void execute() override;
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();

private:
    bool validate() override;

    void execute() override;
};

class BombOrder : public Order {
public:
    BombOrder();

private:

    bool validate() override;

    void execute() override;
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();

private:
    bool validate() override;

    void execute() override;
};

class AirliftOrder : public Order {
public:
    AirliftOrder();

private:
    bool validate() override;

    void execute() override;
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();

private:
    bool validate() override;

    void execute() override;
};

/**
 * This class represents a list of orders for the Risk game
 * An OrdersList is a list of Orders that can be Deploy, Advance, Bomb, Blockade, Airlift, Negotiate.
 */
class OrdersList {

private:
    vector<Order *> orderList;

public:
    OrdersList();

    ~OrdersList();
// TODO: figure out how copy constructor works with inheritance.
//    OrdersList(const OrdersList &original);
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

    void print();

    void executeAll();
};

#endif //RISK_ORDERS_H
