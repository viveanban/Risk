#include <iostream>
#include <algorithm>
#include "Orders.h"

using namespace std;

// Superclass: Order ---------------------------------------------------------------------------------------------------

std::ostream &operator<<(std::ostream &stream, Order &order) {
    return stream << order.description << endl;
}

Order::~Order() = default;

// DeployOrder ---------------------------------------------------------------------------------------------------------
DeployOrder::DeployOrder() { description = "Deploy!"; }

DeployOrder::DeployOrder(const DeployOrder &original) : DeployOrder() {}

DeployOrder &DeployOrder::operator=(const DeployOrder &order) { return *this; }

bool DeployOrder::validate() {
    cout << "Validating deploy order." << endl;
    return true;
}

void DeployOrder::execute() {
    if (validate()) {
        cout << "Executing deploy order." << endl;
    }
}

// AdvanceOrder --------------------------------------------------------------------------------------------------------
AdvanceOrder::AdvanceOrder() { description = "Advance!"; }

AdvanceOrder::AdvanceOrder(const AdvanceOrder &original) : AdvanceOrder() {}

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order) { return *this; }

bool AdvanceOrder::validate() {
    cout << "Validating advance order." << endl;
    return true;

}

void AdvanceOrder::execute() {
    if (validate()) {
        cout << "Executing advance order." << endl;
    }
}

// BombOrder -----------------------------------------------------------------------------------------------------------
BombOrder::BombOrder() { description = "Bomb!"; }

BombOrder::BombOrder(const BombOrder &original) : BombOrder() {}

BombOrder &BombOrder::operator=(const BombOrder &order) { return *this; }

bool BombOrder::validate() {
    cout << "Validating bomb order." << endl;
    return true;

}

void BombOrder::execute() {
    if (validate()) {
        cout << "Executing bomb order." << endl;
    }
}

// BlockadeOrder -------------------------------------------------------------------------------------------------------
BlockadeOrder::BlockadeOrder() { description = "Blockade!"; }

BlockadeOrder::BlockadeOrder(const BlockadeOrder &original) : BlockadeOrder() {}

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) { return *this; }

bool BlockadeOrder::validate() {
    cout << "Validating blockade order." << endl;
    return true;

}

void BlockadeOrder::execute() {
    if (validate()) {
        cout << "Executing blockade order." << endl;
    }
}

// AirliftOrder --------------------------------------------------------------------------------------------------------
AirliftOrder::AirliftOrder() { description = "Airlift!"; }

AirliftOrder::AirliftOrder(const AirliftOrder &original) : AirliftOrder() {}

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order) { return *this; }

bool AirliftOrder::validate() {
    cout << "Validating airlift order." << endl;
    return true;

}

void AirliftOrder::execute() {
    if (validate()) {
        cout << "Executing airlift order." << endl;
    }
}

// NegotiateOrder ------------------------------------------------------------------------------------------------------
NegotiateOrder::NegotiateOrder() { description = "Negotiate!"; }

NegotiateOrder::NegotiateOrder(const NegotiateOrder &original) : NegotiateOrder() {}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order) { return *this; }

bool NegotiateOrder::validate() {
    cout << "Validating negotiate order." << endl;
    return true;
}

void NegotiateOrder::execute() {
    if (validate()) {
        cout << "Executing negotiate order." << endl;
    }
}

// ReinforcementOrder --------------------------------------------------------------------------------------------------

ReinforcementOrder::ReinforcementOrder() { description = "Reinforce!"; }

ReinforcementOrder::ReinforcementOrder(const ReinforcementOrder &original) : ReinforcementOrder() {}

ReinforcementOrder &ReinforcementOrder::operator=(const ReinforcementOrder &order) { return *this; }

bool ReinforcementOrder::validate() {
    cout << "Validating reinforce order." << endl;
    return true;
}

void ReinforcementOrder::execute() {
    if (validate()) {
        cout << "Executing reinforce order." << endl;
    }
}

//--------------------- ORDERS LIST-------------------------------------------------------------------------------------
OrdersList::OrdersList() : orderList() {}

OrdersList::OrdersList(const OrdersList &original) {
    auto newOrderList = vector<Order *>();
    copyOrderList(original.orderList, newOrderList);
    orderList = newOrderList;
}

OrdersList &OrdersList::operator=(const OrdersList &original) {
    auto newOrderList = vector<Order *>();
    copyOrderList(original.orderList, newOrderList);
    orderList = newOrderList;
    return *this;
}

void OrdersList::copyOrderList(const vector<Order *> &originalVector, vector<Order *> &destinationVector) {
    for (auto order : originalVector) {
        if (auto *deployOrder = dynamic_cast<DeployOrder *>(order)) {
            destinationVector.push_back(new DeployOrder(*deployOrder));
        } else if (auto *advanceOrder = dynamic_cast<AdvanceOrder *>(order)) {
            destinationVector.push_back(new AdvanceOrder(*advanceOrder));
        } else if (auto *bombOrder = dynamic_cast<BombOrder *>(order)) {
            destinationVector.push_back(new BombOrder(*bombOrder));
        } else if (auto *blockadeOrder = dynamic_cast<BlockadeOrder *>(order)) {
            destinationVector.push_back(new BlockadeOrder(*blockadeOrder));
        } else if (auto *airliftOrder = dynamic_cast<AirliftOrder *>(order)) {
            destinationVector.push_back(new AirliftOrder(*airliftOrder));
        } else if (auto *negotiateOrder = dynamic_cast<NegotiateOrder *>(order)) {
            destinationVector.push_back(new NegotiateOrder(*negotiateOrder));
        } else if (auto *reinforceOrder = dynamic_cast<ReinforcementOrder *>(order)) {
            destinationVector.push_back(new ReinforcementOrder(*reinforceOrder));
        } else {
            cout << "WARNING: Order of unknown type" << endl;
        }
    }
}

ostream &operator<<(ostream &stream, OrdersList &ordersList) {
    string result = "Orders in list:\n";
    for (Order *o : ordersList.orderList)
        result += o->description + "\n";
    return stream << result << endl;
}

void OrdersList::add(Order *order) {
    orderList.push_back(order);
}

bool OrdersList::remove(Order *order) {
    auto position = find(orderList.begin(), orderList.end(), order);
    if (position != orderList.end()) {
        orderList.erase(position);
        return true;
    }
    cout << "Error deleting order." << endl;
    return false;
}

bool OrdersList::move(Order *order, int destination) {
    if (destination < orderList.size()) {
        auto oldPosition = find(orderList.begin(), orderList.end(), order);
        const int oldIndex = distance(orderList.begin(), oldPosition);

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

void OrdersList::executeAll() {
    for (Order *odr: orderList) {
        odr->execute();
    }
}

vector<Order *> &OrdersList::getOrderList() {
    return this->orderList;
}

OrdersList::~OrdersList() {
    for (auto o: orderList) {
        delete o;
        o = nullptr;
    }
    orderList.clear();
}