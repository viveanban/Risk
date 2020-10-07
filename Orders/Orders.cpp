//
// Created by Viveka Anban on 03-10-2020.
//

#include <iostream>
#include <algorithm>
#include "Orders.h"

using namespace std;

std::ostream &operator<<(std::ostream &stream, Order &order) {
    return stream << order.description << endl;
}

bool DeployOrder::validate() {
    cout << "Validating deploy order." << endl;
    return true;
}

void DeployOrder::execute() {
    if(validate()) {
        cout << "Executing deploy order." << endl;
    }
}
DeployOrder::DeployOrder() { description = "Deploy!";}

bool AdvanceOrder::validate() {
    cout << "Validating advance order." << endl;
    return true;

}

void AdvanceOrder::execute() {
    if(validate()) {
        cout << "Executing advance order." << endl;
    }
}

AdvanceOrder::AdvanceOrder() {description = "Advance!";}

bool BombOrder::validate() {
    cout << "Validating bomb order." << endl;
    return true;

}

void BombOrder::execute() {
    if(validate()) {
        cout << "Executing bomb order." << endl;
    }
}

BombOrder::BombOrder() {description = "Bomb!";}

bool BlockadeOrder::validate() {
    cout << "Validating blockade order." << endl;
    return true;

}

void BlockadeOrder::execute() {
    if(validate()) {
        cout << "Executing blockade order." << endl;
    }
}

BlockadeOrder::BlockadeOrder() {description = "Blockade!";}

bool AirliftOrder::validate() {
    cout << "Validating airlift order." << endl;
    return true;

}

void AirliftOrder::execute() {
    if(validate()) {
        cout << "Executing airlift order." << endl;
    }
}

AirliftOrder::AirliftOrder() {description = "Airlift!";}

bool NegotiateOrder::validate() {
    cout << "Validating negotiate order." << endl;
    return true;
}

void NegotiateOrder::execute() {
    if(validate()) {
        cout << "Executing negotiate order." << endl;
    }
}

NegotiateOrder::NegotiateOrder() {description = "Negotiate!";}

//--------------------- ORDERLIST --------------------------------------------------------------------------------------
OrdersList::OrdersList() : orderList() {}

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
    cout << "Error deleting order." << endl;
    return false;
}
bool OrdersList::move(Order * order, int destination) {
    if (destination < orderList.size()) {
        auto oldPosition = find(orderList.begin(), orderList.end(), order);
        const int oldIndex = distance(orderList.begin(), oldPosition);

        // == orderList.end() means the element was not found
        if (oldPosition != orderList.end() && oldIndex != destination) {
            Order *copy = order;
            orderList.erase(orderList.begin() + oldIndex);
            orderList.insert(orderList.begin() + destination, copy);
            return true;
        }
    }
    cout << "Error moving order, please check indexes." << endl;
    return false;
}

void OrdersList::print() {
    for (Order *o: orderList)
        cout << o->description << endl;
}

void OrdersList::executeAll() {
    for (Order *odr: orderList) {
        odr->execute();
    }
}