//
// Created by Viveka Anban on 03-10-2020.
//

#ifndef RISK_ORDERS_H
#define RISK_ORDERS_H

#include <vector>
#include <iostream>

using namespace std;

class Order {
public:
    string description = "";
    friend ostream &operator<<(ostream &stream, Order &order);

protected:
    // The following methods are pure virtual functions (must be overridden)
    virtual bool validate() =0;

    virtual void execute() =0;
};

class DeployOrder : public Order {
public:
    DeployOrder();

private:
    bool validate();

    void execute();
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();

private:
    bool validate();

    void execute();
};

class BombOrder : public Order {
public:
    BombOrder();

private:

    bool validate();

    void execute();
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();

private:
    bool validate();

    void execute();
};

class AirliftOrder : public Order {
public:
    AirliftOrder();

private:
    bool validate();

    void execute();
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();

private:
    bool validate();

    void execute();
};

class OrdersList {

private:
    vector<Order *> orderList;

public:
    OrdersList();

    void add(Order *order);

    //TODO: Check with TA if it's ok to change method name to remove b/c delete() is reserved
    bool remove(Order *order);

    bool move(Order *order, int destination);

    void print();
};

#endif //RISK_ORDERS_H
