//
// Created by Viveka Anban on 03-10-2020.
//
#include <vector>
#include "Orders.h"

int main() {
    OrdersList list{};
    Order *overToMove = new DeployOrder();
    Order *orderToRemove = new BombOrder();
    list.add(overToMove);
    list.add(new AdvanceOrder());
    list.add(orderToRemove);
    list.add(new BlockadeOrder());
    list.add(new AirliftOrder());
    list.add(new NegotiateOrder());

    // Tests
    cout << "-----------Original OrderList-----------" << endl;
    list.print();
    cout << "-----------Test Move Deploy to 3rd place-----------" << endl;
    list.move(overToMove, 2);
    list.print();
    cout << "-----------Test Move Deploy to 1st place-----------" << endl;
    list.move(overToMove, 0);
    list.print();
    cout << "-----------Test Remove Bomb Order-----------" << endl;
    list.remove(orderToRemove);
    list.print();
    cout << "-----------Test Execute-----------" << endl;
    list.executeAll();
    return 0;
}

