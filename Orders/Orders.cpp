//
// Created by Viveka Anban on 03-10-2020.
//

#include <iostream>
#include "Orders.h"

using namespace std;

std::ostream &operator<<(std::ostream &stream, Order &order) {
    return stream << order.description << endl;
}

bool DeployOrder::validate() {
    return false;
}

void DeployOrder::execute() {
    if(validate()) {
        cout << "Executing deploy order." << endl;
    }
}
DeployOrder::DeployOrder() { description = "Deploy!";}

bool AdvanceOrder::validate() {
    return false;

}

void AdvanceOrder::execute() {
    if(validate()) {
        cout << "Executing advance order." << endl;
    }
}

AdvanceOrder::AdvanceOrder() {description = "Advance!";}

bool BombOrder::validate() {
    return false;

}

void BombOrder::execute() {
    if(validate()) {
        cout << "Executing bomb order." << endl;
    }
}

BombOrder::BombOrder() {description = "Bomb!";}

bool BlockadeOrder::validate() {
    return false;

}

void BlockadeOrder::execute() {
    if(validate()) {
        cout << "Executing blockade order." << endl;
    }
}

BlockadeOrder::BlockadeOrder() {description = "Blockade!";}

bool AirliftOrder::validate() {
    return false;

}

void AirliftOrder::execute() {
    if(validate()) {
        cout << "Executing airlift order." << endl;
    }
}

AirliftOrder::AirliftOrder() {description = "Airlift!";}

bool NegotiateOrder::validate() {
    return false;
}

void NegotiateOrder::execute() {
    if(validate()) {
        cout << "Executing negotiate order." << endl;
    }
}

NegotiateOrder::NegotiateOrder() {description = "Negotiate!";}

//--------------------- ORDERLIST --------------------------------------------------------------------------------------
OrdersList::OrdersList() {}

void OrdersList::add(Order *order) {
    orderList.push_back(order);
}

bool OrdersList::remove(Order *order) {
    auto position = find(orderList.begin(), orderList.end(), order);
    // == orderList.end() means the element was not found
    if (position != orderList.end()) {
        orderList.erase(position);
        return true;
    }
    return false;
}
bool OrdersList::move(Order * order, int destination) {
    auto oldPosition = find(orderList.begin(), orderList.end(), order);
    const int oldIndex = distance(orderList.begin(), oldPosition);

    // == orderList.end() means the element was not found
    if (oldPosition != orderList.end() && oldIndex != destination) {
        Order * copy = order;
        orderList.insert(orderList.begin() + destination, copy);

        if(oldIndex < destination){
            orderList.erase(orderList.begin() + oldIndex);
        }else if(oldIndex > destination) {
            orderList.erase(orderList.begin() + oldIndex + 1);
        }
        return true;
    }
    return false;
}

void OrdersList::print() {
    for (Order *o: orderList)
        cout << o->description << endl;
}
