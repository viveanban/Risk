#include <iostream>
#include <algorithm>
#include "Orders.h"
#include "./../Player/Player.h"
#include "../GameEngine/GameEngine.h"

using namespace std;
// Superclass: Order ---------------------------------------------------------------------------------------------------
Order::Order(string name, int priority, Player *player) : name(name), priority(priority),
                                                          player(player) {}

Order::Order(const Order &original) : name(original.name), priority(original.priority),
                                      player(original.player) {}

Order &Order::operator=(const Order &otherOrder) {
    if (&otherOrder != this) {
        name = otherOrder.name;
        priority = otherOrder.priority;
        //Note: not assigning player to prevent infinite loop caused by circular dependency
    }
    return *this;
}

Order::~Order() = default;

std::ostream &operator<<(std::ostream &stream, Order &order) {
    return stream << order.name << " => " << order.priority << endl;
}

const string &Order::getName() const {
    return name;
}

int Order::getPriority() const {
    return priority;
}

void Order::setPlayer(Player *player) {
    Order::player = player;
}

// DeployOrder ---------------------------------------------------------------------------------------------------------
DeployOrder::DeployOrder() : DeployOrder(nullptr) {}

DeployOrder::DeployOrder(Player *player) : targetTerritory(nullptr), numberOfArmiesToDeploy(0),
                                           Order("Deploy", 1, player) {}


DeployOrder::DeployOrder(const DeployOrder &original) : targetTerritory(original.targetTerritory),
                                                        numberOfArmiesToDeploy(original.numberOfArmiesToDeploy),
                                                        Order(original) {}

DeployOrder &DeployOrder::operator=(const DeployOrder &otherOrder) {
    if (&otherOrder != this) {
        Order::operator=(otherOrder);
        targetTerritory = otherOrder.targetTerritory;
        numberOfArmiesToDeploy = otherOrder.numberOfArmiesToDeploy;
    }
    return *this;
}

bool DeployOrder::validate() {
    // If the target territory does not belong to the player that issued the order, the order is invalid.
    if (targetTerritory->getOwner() != player) {
        cout << player->getPlayerName() << "'s deploy order validation has failed: "
             << "the target territory does not belong to the player that issued the order." << endl;
        return false;
    }
    cout << "Deploy order validation succeeded!" << endl;
    return true;
}

void DeployOrder::execute() {
    if (validate()) {
        // If the target territory belongs to the player that issued the deploy order (validation successful), the selected number of armies is added to the number of armies on that territory.
        targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() + numberOfArmiesToDeploy);
        GameEngine::getInstance()->getGameState()->updateGameState(player, orders_execution, this,nullptr);
    }
}

bool DeployOrder::issue() {
    // This ensures that the numberOfArmiesToDeploy is always smaller or equal than numberOfArmiesInReinforcementPool
    numberOfArmiesToDeploy = (rand() % player->getNumberofArmiesInReinforcementPool()) + 1;

    // Set the target territory to be player's territory with the least amount of unit armies
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    targetTerritory = territoriesToDefend.at(0);

    // Update the priority of the target territory so that it is not at the top of the list for the next deploy order
    targetTerritory->setPriority(targetTerritory->getPriority() + numberOfArmiesToDeploy);

    // Update number of armies
    player->setNumberOfArmiesInReinforcementPool(player->getNumberofArmiesInReinforcementPool() - numberOfArmiesToDeploy);

    // Update order list
    player->getOrders()->add(this);

    return true;
}

Territory *DeployOrder::getTargetTerritory() const {
    return targetTerritory;
}

int DeployOrder::getNumberOfArmiesToDeploy() const {
    return numberOfArmiesToDeploy;
}

// AdvanceOrder --------------------------------------------------------------------------------------------------------
AdvanceOrder::AdvanceOrder() : AdvanceOrder(nullptr) {}

AdvanceOrder::AdvanceOrder(Player *player) : sourceTerritory(nullptr), targetTerritory(nullptr),
                                             numberOfArmiesToAdvance(0), Order("Advance", 4, player) {}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &original) : sourceTerritory(original.sourceTerritory),
                                                           targetTerritory(original.targetTerritory),
                                                           numberOfArmiesToAdvance(original.numberOfArmiesToAdvance),
                                                           Order(original) {}

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &otherOrder) {
    if (&otherOrder != this) {
        Order::operator=(otherOrder);
        sourceTerritory = otherOrder.sourceTerritory;
        targetTerritory = otherOrder.targetTerritory;
        numberOfArmiesToAdvance = otherOrder.numberOfArmiesToAdvance;
    }
    return *this;
}

bool AdvanceOrder::validate() {
    // If the source territory does not belong to the player that issued the order, the order is invalid.
    if (sourceTerritory->getOwner() != player) {
        cout << player->getPlayerName() << "'s advance order validation has failed: "
             << "the source territory does not belong to the player that issued the order." << endl;
        return false;
    }

    if (numberOfArmiesToAdvance > sourceTerritory->getUnitNbr()) {
        cout << player->getPlayerName() << "'s advance order validation has failed: "
             << "the source territory does not have " << numberOfArmiesToAdvance << " army units to advance." << endl;
        return false;
    }

    // Negotiation
    bool canAttackTargetTerritory = find(player->getPlayersNotToAttack().begin(), player->getPlayersNotToAttack().end(),
                                         targetTerritory->getOwner()) == player->getPlayersNotToAttack().end();

    if (!canAttackTargetTerritory) {
        cout << player->getPlayerName() << "'s advance order validation has failed: "
             << "the target territory cannot be attacked because you negotiated with its owner "
             << targetTerritory->getOwner()->getPlayerName() << endl;
        return false;
    }
    cout << "Advance order validation succeeded!" << endl;
    return true;
}

void AdvanceOrder::execute() {
    if (validate()) {
        //Transfer army units from src to trg territory if both are owned by issuing player
        if (sourceTerritory->getOwner() == player && targetTerritory->getOwner() == player) {
            sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfArmiesToAdvance);
            targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() + numberOfArmiesToAdvance);

        } else { // Attack target territory
            int numberOfTargetUnitsKilled = 0;
            for (int unit = 1;
                // Source cannot kill more than the number of armies in the target territory
                 unit <= numberOfArmiesToAdvance && numberOfTargetUnitsKilled < targetTerritory->getUnitNbr();
                 unit++) {
                // Source has 60% chance of killing a target army unit
                bool targetUnitKilled = kill(60);
                if (targetUnitKilled) numberOfTargetUnitsKilled++;
            }

            int numberOfSourceUnitsKilled = 0;
            for (int unit = 1;
                // Target cannot kill more than the number of armies advanced by src
                 unit <= targetTerritory->getUnitNbr() && numberOfSourceUnitsKilled < numberOfArmiesToAdvance;
                 unit++) {
                // Target has 70% chance of killing a source army unit
                bool sourceUnitKilled = kill(70);
                if (sourceUnitKilled) numberOfSourceUnitsKilled++;
            }

            int targetArmyUnitsLeft = targetTerritory->getUnitNbr() - numberOfTargetUnitsKilled;

            // Source Territory has conquered the Target Territory
            if (targetArmyUnitsLeft == 0) {
                // Updating the army unit numbers for each territory
                sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfArmiesToAdvance);
                targetTerritory->setUnitNbr(numberOfArmiesToAdvance - numberOfSourceUnitsKilled);

                // Transfer ownership
                targetTerritory->setOwner(player);
            } else { // Target is not conquered
                // Updating the army unit numbers for each territory
                sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfSourceUnitsKilled);
                targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() - numberOfTargetUnitsKilled);
            }
        }
        GameEngine::getInstance()->getGameState()->updateGameState(player, orders_execution, this,nullptr);
    }
}

bool AdvanceOrder::issue() {
    // Determine src territory
    sourceTerritory = player->getTerritories().at(rand() % player->getTerritories().size());

    // Determine target territory
    bool attack = rand() % 2;
    advanceOrderType = attack? AdvanceOrderType::attack : AdvanceOrderType::transfer;
    vector<Territory *> territoriesToChooseFrom = attack ? player->toAttack(sourceTerritory) : player->toDefend(
            sourceTerritory);
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getName()
             << " because this player has no territories to " << (attack ? "attack" : "transfer") << endl;
        return false;
    }

    targetTerritory = territoriesToChooseFrom.at(0);

    // Determine number of armies to advance
    numberOfArmiesToAdvance = (rand() % (sourceTerritory->getPriority() > 0 ? sourceTerritory->getPriority() : 6)) + 1;

    // Update priority
    sourceTerritory->setPriority(sourceTerritory->getPriority() - numberOfArmiesToAdvance);
    targetTerritory->setPriority(attack ?
                                 targetTerritory->getPriority() - numberOfArmiesToAdvance :
                                 targetTerritory->getPriority() + numberOfArmiesToAdvance);

    // Update order list
    player->getOrders()->add(this);

    return true;
}

bool AdvanceOrder::kill(int probabilityToKill) {
    int randomValue = (rand() % 100) + 1; // [1, 100]
    return randomValue <= probabilityToKill;
}

Territory *AdvanceOrder::getSourceTerritory() const {
    return sourceTerritory;
}

Territory *AdvanceOrder::getTargetTerritory() const {
    return targetTerritory;
}

int AdvanceOrder::getNumberOfArmiesToAdvance() const {
    return numberOfArmiesToAdvance;
}

AdvanceOrderType AdvanceOrder::getAdvanceOrderType() const {
    return advanceOrderType;
}

// BombOrder -----------------------------------------------------------------------------------------------------------
BombOrder::BombOrder() : BombOrder(nullptr) {}

BombOrder::BombOrder(Player *player) : targetTerritory(nullptr), Order("Bomb", 4, player) {}

BombOrder::BombOrder(const BombOrder &original) : targetTerritory(original.targetTerritory), Order(original) {}

BombOrder &BombOrder::operator=(const BombOrder &otherOrder) {
    if (&otherOrder != this) {
        Order::operator=(otherOrder);
        targetTerritory = otherOrder.targetTerritory;
    }
    return *this;
}

bool BombOrder::validate() {
    // If the target belongs to the player that issued the order, the order is invalid.
    if (targetTerritory->getOwner() == player) {
        cout << player->getPlayerName() << "'s bomb order validation has failed: "
             << "the target territory belongs to the player that issued the order." << endl;
        return false;
    }
    bool canAttackTargetTerritory = find(player->getPlayersNotToAttack().begin(), player->getPlayersNotToAttack().end(),
                                         targetTerritory->getOwner()) == player->getPlayersNotToAttack().end();
    if (!canAttackTargetTerritory) {
        cout << player->getPlayerName() << "'s bomb order validation has failed: "
             << "the target territory belongs to a player that is in negotiation with the attacking player." << endl;
        return false;
    }
    cout << "Bomb order validation succeeded!" << endl;

    return true;

}

void BombOrder::execute() {
    if (validate()) {
        // If the target belongs to an enemy player, half of the armies are removed from this territory.
        targetTerritory->setUnitNbr((int) (targetTerritory->getUnitNbr() / 2));
        GameEngine::getInstance()->getGameState()->updateGameState(player, orders_execution, this,nullptr);
    }
}

bool BombOrder::issue() {
    // Randomly determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getName() << " because this player has no territories to attack." << endl;
        return false;
    }

    targetTerritory = territoriesToAttack.at(rand() % territoriesToAttack.size());

    // Update priority
    targetTerritory->setPriority(targetTerritory->getPriority() / 2);

    // Update order list
    player->getOrders()->add(this);

    return true;
}

Territory *BombOrder::getTargetTerritory() const {
    return targetTerritory;
}

// BlockadeOrder -------------------------------------------------------------------------------------------------------
BlockadeOrder::BlockadeOrder() : BlockadeOrder(nullptr) {}

BlockadeOrder::BlockadeOrder(Player *player) : targetTerritory(nullptr), Order("Blockade", 3, player) {}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &original) : targetTerritory(original.targetTerritory),
                                                              Order(original) {}

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &otherOrder) {
    if (&otherOrder != this) {
        Order::operator=(otherOrder);
        targetTerritory = otherOrder.targetTerritory;
    }
    return *this;
}

bool BlockadeOrder::validate() {
    // If the target territory belongs to an enemy player, the order is declared invalid
    if (targetTerritory->getOwner() != player) {
        cout << player->getPlayerName() << "'s blockade order validation has failed: "
             << "the target territory does not belong to the player that issued the order." << endl;
        return false;
    }
    cout << "Blockade order validation succeeded!" << endl;
    return true;

}

void BlockadeOrder::execute() {
    if (validate()) {
        // If the target territory belongs to the player issuing the order, the number of armies on the territory is
        // doubled and the ownership of the territory is transferred to the Neutral player.
        targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() * 2);
        targetTerritory->setOwner(Player::neutralPlayer);
        GameEngine::getInstance()->getGameState()->updateGameState(player, orders_execution, this,nullptr);
    }
}

bool BlockadeOrder::issue() {
    // Determine target territory to be the player's territory with the most army units
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getName() << " because this player has no territories to defend." << endl;
        return false;
    }

    targetTerritory = territoriesToDefend.at(territoriesToDefend.size() - 1);

    // Update priority
    targetTerritory->setPriority(targetTerritory->getPriority() * 2);

    // Update order list
    player->getOrders()->add(this);
    return true;
}

Territory *BlockadeOrder::getTargetTerritory() const {
    return targetTerritory;
}

// AirliftOrder --------------------------------------------------------------------------------------------------------
AirliftOrder::AirliftOrder() : AirliftOrder(nullptr) {}

AirliftOrder::AirliftOrder(Player *player) : sourceTerritory(nullptr), targetTerritory(nullptr),
                                             numberOfArmiesToAirlift(0), Order("Airlift", 2, player) {}

AirliftOrder::AirliftOrder(const AirliftOrder &original) : sourceTerritory(original.sourceTerritory),
                                                           targetTerritory(original.targetTerritory),
                                                           numberOfArmiesToAirlift(original.numberOfArmiesToAirlift),
                                                           Order(original) {}

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &otherOrder) {
    if (&otherOrder != this) {
        Order::operator=(otherOrder);
        sourceTerritory = otherOrder.sourceTerritory;
        targetTerritory = otherOrder.targetTerritory;
        numberOfArmiesToAirlift = otherOrder.numberOfArmiesToAirlift;
    }
    return *this;
}

bool AirliftOrder::validate() {
    if (sourceTerritory->getOwner() != player) {
        cout << player->getPlayerName() << "'s airlift order validation has failed: "
             << "the source territory does not belong to the player that issued the order." << endl;
        return false;
    }
    if (targetTerritory->getOwner() != player) {
        cout << player->getPlayerName() << "'s airlift order validation has failed: "
             << "the target territory does not belong to the player that issued the order." << endl;
        return false;
    }

    if (numberOfArmiesToAirlift > sourceTerritory->getUnitNbr()) {
        cout << player->getPlayerName() << "'s airlift order validation has failed: "
             << "the source territory does not have enough armies to airlift." << endl;
        return false;
    }
    cout << "Airlift order validation succeeded!" << endl;
    return true;
}

void AirliftOrder::execute() {
    if (validate()) {
        // Transfer armies to target territory
        sourceTerritory->setUnitNbr(sourceTerritory->getUnitNbr() - numberOfArmiesToAirlift);
        targetTerritory->setUnitNbr(targetTerritory->getUnitNbr() + numberOfArmiesToAirlift);
        GameEngine::getInstance()->getGameState()->updateGameState(player, orders_execution, this,nullptr);
    }
}

bool AirliftOrder::issue() {
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << getName() << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine src territory
    sourceTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());

    // Determine target territory
    targetTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());

    // Determine number of armies to advance
    numberOfArmiesToAirlift = (rand() % (sourceTerritory->getPriority() > 0 ? sourceTerritory->getPriority() : 6)) + 1;

    // Update priority
    sourceTerritory->setPriority(sourceTerritory->getPriority() - numberOfArmiesToAirlift);
    targetTerritory->setPriority(targetTerritory->getPriority() + numberOfArmiesToAirlift);

    // Update order list
    player->getOrders()->add(this);

    return true;
}

Territory *AirliftOrder::getSourceTerritory() const {
    return sourceTerritory;
}

Territory *AirliftOrder::getTargetTerritory() const {
    return targetTerritory;
}

int AirliftOrder::getNumberOfArmiesToAirlift() const {
    return numberOfArmiesToAirlift;
}

// NegotiateOrder ------------------------------------------------------------------------------------------------------
NegotiateOrder::NegotiateOrder() : NegotiateOrder(nullptr) {}

NegotiateOrder::NegotiateOrder(Player *player) : targetPlayer(nullptr), Order("Negotiate", 4, player) {}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &original) : targetPlayer(original.targetPlayer), Order(original) {}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &otherOrder) {
    if (&otherOrder != this) {
        Order::operator=(otherOrder);
        targetPlayer = otherOrder.targetPlayer;
    }
    return *this;
}

bool NegotiateOrder::validate() {
    // If the target is the player issuing the order, then the order is invalid.
    if (targetPlayer == player) {
        cout << player->getPlayerName() << "'s negotiate order validation has failed: "
             << "the target player and the source player are the same." << endl;
        return false;
    }
    cout << "Negotiate order order validation succeeded!" << endl;
    return true;
}

void NegotiateOrder::execute() {
    if (validate()) {
        player->getPlayersNotToAttack().insert(targetPlayer);
        targetPlayer->getPlayersNotToAttack().insert(player);
        GameEngine::getInstance()->getGameState()->updateGameState(player, orders_execution, this,nullptr);
    }
}

bool NegotiateOrder::issue() {
    //Determine a random enemy player
    vector<Player *> players = GameEngine::getInstance()->getPlayers();
    if (players.size() < 2) {
        cout << "Cannot play a negotiate order with only one player!" << endl;
        return false;
    }

    do {
        targetPlayer = players.at(rand() % players.size());
    } while (targetPlayer == player);


    // Update order list
    player->getOrders()->add(this);

    return true;
}

Player *NegotiateOrder::getTargetPlayer() const {
    return targetPlayer;
}

//--------------------- ORDERS LIST-------------------------------------------------------------------------------------
OrdersList::OrdersList() : orderList() {}

OrdersList::OrdersList(const OrdersList &original) {
    auto newOrderList = vector<Order *>();
    copyOrderList(original.orderList, newOrderList);
    orderList = newOrderList;
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
        } else {
            cout << "WARNING: Order of unknown type" << endl;
        }
    }
}

OrdersList &OrdersList::operator=(const OrdersList &original) {
    //deleting old orderList before assigning new one
    for(auto o: orderList){
        delete o;
    }
    orderList.clear();

    auto newOrderList = vector<Order *>();
    copyOrderList(original.orderList, newOrderList);
    orderList = newOrderList;
    return *this;
}

ostream &operator<<(ostream &stream, OrdersList &ordersList) {
    string result;
    for (Order *o : ordersList.orderList)
        result += o->getName() + "\n";
    return stream << result;
}

void OrdersList::add(Order *order) {
    orderList.push_back(order);
}

void OrdersList::remove(Order *order) {
    auto position = find(orderList.begin(), orderList.end(), order);
    if (position != orderList.end()) {
        orderList.erase(position);
        delete order;
        order = nullptr;
    } else {
        cerr << "Remove order operation failed: this order does not belong in the Player's order list." << endl;
    }
}

void OrdersList::move(Order *order, int destination) {
    if (destination < orderList.size()) {
        auto oldPosition = find(orderList.begin(), orderList.end(), order);
        const int oldIndex = distance(orderList.begin(), oldPosition);

        if (oldPosition != orderList.end() && oldIndex != destination) {
            Order *copy = order;
            orderList.erase(orderList.begin() + oldIndex);
            orderList.insert(orderList.begin() + destination, copy);
        }
    } else {
        cerr << "Move order operation failed, please check indexes." << endl;
    }
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
        cout << "Deleted order" << endl;
    }
    orderList.clear();
}
