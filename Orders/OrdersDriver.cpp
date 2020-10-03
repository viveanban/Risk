//
// Created by Viveka Anban on 03-10-2020.
//
#include <vector>
#include "Orders.h"

int main() {
    OrdersList list{};
    Order * overToMove = new DeployOrder();
    Order * orderToRemove = new BombOrder();
    list.add(overToMove);
    list.add(new AdvanceOrder());
    list.add(orderToRemove);
    list.add(new BlockadeOrder());
    list.add(new AirliftOrder());
    list.add(new NegotiateOrder());

    list.print();

    list.move(overToMove, 3);
    list.print();

    cout << "remove" << endl;
    list.remove(orderToRemove);
    list.print();
    return 0;
}

