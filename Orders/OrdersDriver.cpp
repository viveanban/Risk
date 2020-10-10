//#include <vector>
//#include "Orders.h"
//
//int main() {
//    OrdersList *list = new OrdersList();
//    Order *overToMove = new DeployOrder();
//    Order *orderToRemove = new BombOrder();
//    list->add(overToMove);
//    list->add(new AdvanceOrder());
//    list->add(orderToRemove);
//    list->add(new BlockadeOrder());
//    list->add(new AirliftOrder());
//    list->add(new NegotiateOrder());
//
//    // Copy constructor
//    OrdersList *list2 = new OrdersList(*list);
//
//    // Tests
//    cout << "-----------Original OrderList-----------" << endl;
//    cout << *list << endl;
//    cout << "-----------Test Move Deploy to 3rd place-----------" << endl;
//    list->move(overToMove, 2);
//    cout << *list << endl;
//    cout << "-----------Test Move Deploy to 1st place-----------" << endl;
//    list->move(overToMove, 0);
//    cout << *list << endl;
//    cout << "-----------Test Remove Bomb Order-----------" << endl;
//    list->remove(orderToRemove);
//    cout << *list << endl;
//    cout << "-----------Test Execute-----------" << endl;
//    list->executeAll();
//
//    // Assignment operator
//    *list2 = *list;
//
//    // Destructors
//    delete list;
//    list = nullptr;
//    delete list2;
//    list2 = nullptr;
//
//    return 0;
//}