#include <iostream>
#include <algorithm>
#include "Orders.h"
#include "./../Player/Player.h"

using namespace std;

// Superclass: Order ---------------------------------------------------------------------------------------------------

Order::Order(string description, int priority, Player *player) : description(description), priority(priority),
                                                                 player(player) {}

std::ostream &operator<<(std::ostream &stream, Order &order) {
    return stream << order.description << " => " << order.priority << endl;
}

const string &Order::getDescription() const {
    return description;
}

int Order::getPriority() const {
    return priority;
}

Order::~Order() = default;

// DeployOrder ---------------------------------------------------------------------------------------------------------
DeployOrder::DeployOrder(Player *player) : targetTerritory(nullptr), numberOfArmiesToDeploy(0),
                                           Order("Deploy!", 1, player) {}

//TODO: Fix copy ctor comme du monde
DeployOrder::DeployOrder(const DeployOrder &original) : DeployOrder(original.player) {}

DeployOrder &DeployOrder::operator=(const DeployOrder &order) { return *this; }

bool DeployOrder::validate() {
//    If the target territory does not belong to the player that issued the order, the order is invalid.
    if (targetTerritory->getOwner() != player) {
        cout << "Deploy order validation has failed:"
             << "the target territory does not belong to the player that issued the order." << endl;
        return false;
    }
    cout << "Deploy order validation is successful!" << endl;
    return true;
}

void DeployOrder::execute() {
    if (validate()) {
        // If the target territory belongs to the player that issued the deploy order (validation successful), the selected number of armies is added to the number of armies on that territory.
        targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() + numberOfArmiesToDeploy);

        //Output effect of the deployOrder
        cout << "Executing deploy order." << endl;
        cout << numberOfArmiesToDeploy << " number of armies have been deployed to "
             << targetTerritory->getTerritoryName() << endl;
        cout << targetTerritory->getTerritoryName() << " territory now has " << targetTerritory->getUnitNbr()
             << " army units." << endl;
    }
}

void DeployOrder::issue() {
    // This ensures that the numberOfArmiesToDeploy is always smaller or equal than numberOfArmies
    numberOfArmiesToDeploy = (rand() % player->getNumberofArmies()) + 1;

    // Set the target territory to be player's territory with the least amount of unit armies
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getDescription() << endl;
        return;
    }

    targetTerritory = territoriesToDefend.at(0);

    // Update the priority of the target territory so that it is not at the top of the list for the next deploy order
    targetTerritory->setPriority(targetTerritory->getPriority() + numberOfArmiesToDeploy);

    // Update number of armies
    player->setNumberOfArmies(player->getNumberofArmies() - numberOfArmiesToDeploy);

    // Update order list
    player->getOrders()->add(this);
}

// AdvanceOrder --------------------------------------------------------------------------------------------------------
AdvanceOrder::AdvanceOrder(Player *player) : sourceTerritory(nullptr), targetTerritory(nullptr),
                                             numberOfArmiesToAdvance(0), Order("Advance!", 5, player) {}

//TODO: Fix copy ctor comme du monde
AdvanceOrder::AdvanceOrder(const AdvanceOrder &original) : AdvanceOrder(original.player) {}

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order) { return *this; }

bool AdvanceOrder::validate() {
//    If the source territory does not belong to the player that issued the order, the order is invalid.
    if (sourceTerritory->getOwner() != player) {
        cout << "Advance order validation has failed:"
             << "the source territory does not belong to the player that issued the order." << endl;
        return false;
    }

    if (numberOfArmiesToAdvance > sourceTerritory->getUnitNbr()) {
        cout << "Advance order validation has failed:"
             << "the source territory does not have " << numberOfArmiesToAdvance << " army units to advance." << endl;
        return false;
    }
    cout << "Advance order validation is successful!" << endl;
    return true;
}

void AdvanceOrder::execute() {
    if (validate()) {
        cout << "Executing advance order." << endl;

        //Transfer army units from src to trg territory if both are owned by issuing player
        if (sourceTerritory->getOwner() == player && targetTerritory->getOwner() == player) {
            sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfArmiesToAdvance);
            targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() + numberOfArmiesToAdvance);

            cout << sourceTerritory->getTerritoryName() << " has transferred " << numberOfArmiesToAdvance
                 << " army units to "
                 << targetTerritory->getTerritoryName() << endl;
            cout << sourceTerritory->getTerritoryName() << " now has " << sourceTerritory->getUnitNbr()
                 << " army units." << endl;
            cout << targetTerritory->getTerritoryName() << " now has " << targetTerritory->getUnitNbr()
                 << " army units." << endl;;
        } else { // Attack target territory
            int numberOfTargetUnitsKilled;
            for (int unit = 1;
                // Source cannot kill more than the number of armies in the target territory
                 unit <= numberOfArmiesToAdvance && numberOfTargetUnitsKilled < targetTerritory->getUnitNbr();
                 unit++) {
                // Source has 60% chance of killing a target army unit
                bool targetUnitKilled = kill(60);
                if (targetUnitKilled) numberOfTargetUnitsKilled++;
            }

            int numberOfSourceUnitsKilled;
            for (int unit = 1;
                // Target cannot kill more than the number of armies advanced by src
                 unit <= targetTerritory->getUnitNbr() && numberOfSourceUnitsKilled < numberOfArmiesToAdvance;
                 unit++) {
                // Target has 70% chance of killing a source army unit
                bool sourceUnitKilled = kill(70);
                if (sourceUnitKilled) numberOfSourceUnitsKilled++;
            }

            int targetArmyUnitsLeft = targetTerritory->getUnitNbr() - numberOfTargetUnitsKilled;

            //Source Territory has conquered the Target Territory
            if (targetArmyUnitsLeft == 0) {
                // Updating the army unit numbers for each territory
                sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfArmiesToAdvance);
                targetTerritory->setUnitNbr(numberOfArmiesToAdvance - numberOfSourceUnitsKilled);

                //Transfer ownership
                targetTerritory->setOwner(player);

                //TODO: Give new card to player (access deck)
                // Card * drawnCard = GameEngine::deck->draw();
                // player->getHandofCards()->addCard(drawnCard)

                //TODO: Output effect
            } else { // Target is not conquered
                // Updating the army unit numbers for each territory
                sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfSourceUnitsKilled);
                targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() - numberOfTargetUnitsKilled);
                //TODO: Output effect
            }
        }
    }
}

void AdvanceOrder::issue() {
    // Determine src territory
    sourceTerritory = player->getTerritories().at(rand() % player->getTerritories().size());

    // Determine target territory
    bool attack = rand() % 2;
    vector<Territory *> territoriesToChooseFrom = attack ? player->toAttack(sourceTerritory) : player->toDefend(
            sourceTerritory);
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getDescription() << endl;
        return;
    }

    targetTerritory = territoriesToChooseFrom.at(0);

    // Determine number of armies to advance
    numberOfArmiesToAdvance = (rand() % sourceTerritory->getUnitNbr()) + 1;

    // Update priority
    targetTerritory->setPriority(attack ?
                                 targetTerritory->getPriority() - numberOfArmiesToAdvance :
                                 targetTerritory->getPriority() + numberOfArmiesToAdvance);

    // Update order list
    player->getOrders()->add(this);
}

bool AdvanceOrder::kill(int probabilityToKill) {
    int randomValue = (rand() % 100) + 1; // [1, 100]
    return randomValue <= probabilityToKill;
}

// BombOrder -----------------------------------------------------------------------------------------------------------
BombOrder::BombOrder(Player *player) : Order("Bomb!", 5, player) {}

//TODO: Fix copy ctor comme du monde
BombOrder::BombOrder(const BombOrder &original) : BombOrder(original.player) {}

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

void BombOrder::issue() {
    // Randomly determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getDescription() << endl;
        return;
    }

    targetTerritory = territoriesToAttack.at(rand() % territoriesToAttack.size());

    // Update order list
    player->getOrders()->add(this);
}

// BlockadeOrder -------------------------------------------------------------------------------------------------------
BlockadeOrder::BlockadeOrder(Player *player) : targetTerritory(nullptr), Order("Blockade!", 3, player) {}

//TODO: Fix copy ctor comme du monde
BlockadeOrder::BlockadeOrder(const BlockadeOrder &original) : BlockadeOrder(original.player) {}

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) { return *this; }

bool BlockadeOrder::validate() {
    cout << "Validating blockade order." << endl;
    return true;

}

// you give up territory but it's stronger. It can be attacked. Creating a wall.
// you can win the game even tho there's neutral territories
// double the armi unit on the target territory
void BlockadeOrder::execute() {
    if (validate()) {
        cout << "Executing blockade order." << endl;
    }
}

void BlockadeOrder::issue() {
    // Determine target territory to be the player's territory with the most army units
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getDescription() << endl;
        return;
    }

    targetTerritory = territoriesToDefend.at(territoriesToDefend.size() - 1);

    // Update order list
    player->getOrders()->add(this);
}

// AirliftOrder --------------------------------------------------------------------------------------------------------
AirliftOrder::AirliftOrder(Player *player) : sourceTerritory(nullptr), targetTerritory(nullptr),
                                             numberOfArmiesToAirlift(0), Order("Airlift!", 2, player) {}

//TODO: Fix copy ctor comme du monde
AirliftOrder::AirliftOrder(const AirliftOrder &original) : AirliftOrder(original.player) {}

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order) { return *this; }

bool AirliftOrder::validate() {
    if (sourceTerritory->getOwner() != player) {
        cout << "Airlift order validation has failed:"
             << "the source territory does not belong to the player that issued the order." << endl;
        return false;
    }

    if (targetTerritory->getOwner() != player) {
        cout << "Airlift order validation has failed:"
             << "the target territory does not belong to the player that issued the order." << endl;
        return false;
    }

    if (numberOfArmiesToAirlift > sourceTerritory->getUnitNbr()) {
        cout << "Airlift order validation has failed:"
             << "the source territory does not have enough armies to airlift." << endl;
        return false;
    }

    cout << "Airlift order validation is successful!" << endl;
    return true;
}

void AirliftOrder::execute() {
    if (validate()) {
        // Transfer armies to target territory
        sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfArmiesToAirlift);
        targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() + numberOfArmiesToAirlift);

        //Output effect of the airlift order
        cout << "Executing airlift order." << endl;
        cout << numberOfArmiesToAirlift << " number of armies have been airlifted to "
             << targetTerritory->getTerritoryName() << " from " << sourceTerritory->getTerritoryName() << endl;
        cout << sourceTerritory->getTerritoryName() << " territory now has " << sourceTerritory->getUnitNbr()
             << " army units." << endl;
        cout << targetTerritory->getTerritoryName() << " territory now has " << targetTerritory->getUnitNbr()
             << " army units." << endl;
    }
}

void AirliftOrder::issue() {
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getDescription() << endl;
        return;
    }

    // Determine src territory
    sourceTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());

    // Determine target territory
    targetTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());

    // Determine number of armies to advance
    numberOfArmiesToAirlift = (rand() % sourceTerritory->getUnitNbr()) + 1;

    // Update priority
    sourceTerritory->setPriority(sourceTerritory->getPriority() - numberOfArmiesToAirlift);
    targetTerritory->setPriority(targetTerritory->getPriority() + numberOfArmiesToAirlift);

    // Update order list
    player->getOrders()->add(this);
}

// NegotiateOrder ------------------------------------------------------------------------------------------------------
NegotiateOrder::NegotiateOrder(Player *player) : Order("Negotiate!", 4, player) {}

//TODO: Fix copy ctor comme du monde
NegotiateOrder::NegotiateOrder(const NegotiateOrder &original) : NegotiateOrder(original.player) {}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order) { return *this; }

bool NegotiateOrder::validate() {
    cout << "Validating negotiate order." << endl;
    return true;
}

void NegotiateOrder::execute() {
    if (validate()) {
        // set random player in your cannot attack list
        // set yourself in their cannot attack list
        cout << "Executing negotiate order." << endl;
    }
}

void NegotiateOrder::issue() {
    // TODO: Determine a random enemy player
//    targetPlayer = enemies.at(rand() % enemies.size());

    // Update order list
    player->getOrders()->add(this);
}

//--------------------- ORDERS LIST-------------------------------------------------------------------------------------
OrdersList::OrdersList() : orderList() {}

OrdersList::OrdersList(const OrdersList &original) {
    auto newOrderList = vector<Order *>();
    copyOrderList(original.orderList, newOrderList);
    orderList = newOrderList;
}

void OrdersList::copyOrderList(const vector<Order *> &originalVector, vector<Order *> &destinationVector) {
    //TODO: Uncomment once the copy ctors are implemented
//    for (auto order : originalVector) {
//        if (auto *deployOrder = dynamic_cast<DeployOrder *>(order)) {
//            destinationVector.push_back(new DeployOrder(*deployOrder));
//        } else if (auto *advanceOrder = dynamic_cast<AdvanceOrder *>(order)) {
//            destinationVector.push_back(new AdvanceOrder(*advanceOrder));
//        } else if (auto *bombOrder = dynamic_cast<BombOrder *>(order)) {
//            destinationVector.push_back(new BombOrder(*bombOrder));
//        } else if (auto *blockadeOrder = dynamic_cast<BlockadeOrder *>(order)) {
//            destinationVector.push_back(new BlockadeOrder(*blockadeOrder));
//        } else if (auto *airliftOrder = dynamic_cast<AirliftOrder *>(order)) {
//            destinationVector.push_back(new AirliftOrder(*airliftOrder));
//        } else if (auto *negotiateOrder = dynamic_cast<NegotiateOrder *>(order)) {
//            destinationVector.push_back(new NegotiateOrder(*negotiateOrder));
//        } else {
//            cout << "WARNING: Order of unknown type" << endl;
//        }
//    }
}

OrdersList &OrdersList::operator=(const OrdersList &original) {
    auto newOrderList = vector<Order *>();
    copyOrderList(original.orderList, newOrderList);
    orderList = newOrderList;
    return *this;
}

ostream &operator<<(ostream &stream, OrdersList &ordersList) {
    string result = "Orders in list:\n";
    for (Order *o : ordersList.orderList)
        result += o->getDescription() + "\n";
    return stream << result << endl;
}

void OrdersList::add(Order *order) {
    orderList.push_back(order);
}

bool OrdersList::remove(Order *order) {
    auto position = find(orderList.begin(), orderList.end(), order);
    if (position != orderList.end()) {
        orderList.erase(position);
        delete order;
        order = nullptr;
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

vector<Order *> &OrdersList::getOrderList() {
    return this->orderList;
}

void OrdersList::sortOrderListByPriority() {
    sort(orderList.begin(), orderList.end(), [](Order *lhs, Order *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
}

OrdersList::~OrdersList() {
    for (auto o: orderList) {
        delete o;
        o = nullptr;
    }
    orderList.clear();
}
