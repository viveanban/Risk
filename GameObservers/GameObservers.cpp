#include "GameObservers.h"
#include <iomanip>


// SUBJECT
Subject::Subject() {
    observers = new list<Observer *>;
}

Subject::~Subject() {
    delete observers;
}

void Subject::attach(Observer *o) {
    observers->push_back(o);
};

void Subject::detach(Observer *o) {
    observers->remove(o);
};

void Subject::notify() {
    list<Observer *>::iterator i = observers->begin();
    for (; i != observers->end(); ++i)
        (*i)->update();
};

//PHASE OBSERVER
// TODO: add default constructor?
PhaseObserver::PhaseObserver(GameState *currGameState) : currentGameState(currGameState) {}

PhaseObserver::~PhaseObserver() {
//TODO: implement destructor}
}

PhaseObserver::PhaseObserver(const PhaseObserver &original) {
//TODO: implement copy constructor
}

PhaseObserver &PhaseObserver::operator=(const PhaseObserver &otherObserver) {
//TODO: implement operator
    currentGameState = otherObserver.currentGameState;
    return *this;
}

void PhaseObserver::update() {

    if (currentGameState->getCurrentPlayer() != nullptr && currentGameState->getCurrentPhase() != NULL) {
        displayPhaseUpdates();
    } else {
        cout << "ERROR OCCURED WHILE TRYING TO UPDATE PHASE OBSERVER. NULL VALUES FOUND" << endl;
    }
}

void PhaseObserver::displayPhaseUpdates() {
    cout << currentGameState->getCurrentPlayer()->getPlayerName() << ": " << getPhaseText() << " Phase." << endl;
    displaySpecialInformation();

}

string PhaseObserver::getPhaseText() {
    switch (currentGameState->getCurrentPhase()) {
        case reinforcement:
            return "Reinforcement";
        case issuing_orders:
            return "Issue orders";
        case orders_execution:
            return "Orders execution";
        default:
            return "Unknown State";
    }
}

void PhaseObserver::displaySpecialInformation() {
    switch (currentGameState->getCurrentPhase()) {
        case reinforcement:
            printReinforcementinfo();
            break;
        case issuing_orders:
        case orders_execution:
            //TODO: VERIFY IF NEW ORDER IS always AT THE BACK!!
            printOrderInfo(currentGameState->getCurrentPlayer()->getOrders()->getOrderList().back());
            break;
    }
}

void PhaseObserver::printReinforcementinfo() const {
    cout << currentGameState->getCurrentPlayer()->getPlayerName() << " received "
         << currentGameState->getCurrentPlayer()->getNumberofArmies() << " number of armies." << endl;
}

void PhaseObserver::printOrderInfo(Order *order) {

    if (order == nullptr)
        return;
    if (auto *deployOrder = dynamic_cast<DeployOrder *>(order)) {
        printDeployOrderInfo(deployOrder);
    } else if (auto *advanceOrder = dynamic_cast<AdvanceOrder *>(order)) {
        printAdvanceOrder(advanceOrder);
    } else if (auto *bombOrder = dynamic_cast<BombOrder *>(order)) {
        printBombOrder(bombOrder);
    } else if (auto *blockadeOrder = dynamic_cast<BlockadeOrder *>(order)) {
        printBlockadeOrder(blockadeOrder);
    } else if (auto *airliftOrder = dynamic_cast<AirliftOrder *>(order)) {
        printAirliftOrder(airliftOrder);
    } else if (auto *negotiateOrder = dynamic_cast<NegotiateOrder *>(order)) {
        printNegotiateOrder(negotiateOrder);
    } else {
        cout << "WARNING: Order of unknown type" << endl;
    }
}

void PhaseObserver::printBombOrder(BombOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued a Bomb order by playing a Bomb card on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a Bomb order on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " unit number." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printAdvanceOrder(AdvanceOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued an advance order from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to " << pOrder->getTargetTerritory()
                 << " involving " << pOrder->getNumberOfArmiesToAdvance() << " number of armies." << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed an advance order from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to " << pOrder->getTargetTerritory()
                 << " involving " << pOrder->getNumberOfArmiesToAdvance() << " number of armies." << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " is now owned by " <<
                 pOrder->getTargetTerritory()->getOwner()->getPlayerName() << " and has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies in it." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printDeployOrderInfo(DeployOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued a deploy order of "
                 << pOrder->getNumberOfArmiesToDeploy() << " armies on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued a deploy order of"
                 << pOrder->getNumberOfArmiesToDeploy() << " armies on "
                 << pOrder->getTargetTerritory()->getTerritoryName()
                 << ". " << currentGameState->getCurrentPlayer()->getPlayerName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies on this territory." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printBlockadeOrder(BlockadeOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued a blockade order by using a blockade card on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;

            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a blockade order on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies in it and is owned by "
                 << pOrder->getTargetTerritory()->getOwner()->getPlayerName() << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printAirliftOrder(AirliftOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued an Airlift order by using an Airlift card from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to "
                 << pOrder->getTargetTerritory()->getTerritoryName() << " with " << pOrder->getNumberOfArmiesToAirlift()
                 << " armies." << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed an Airlift order from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to "
                 << pOrder->getTargetTerritory()->getTerritoryName() << " with " << pOrder->getNumberOfArmiesToAirlift()
                 << " armies." << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies in it and is owned by "
                 << pOrder->getTargetTerritory()->getOwner()->getPlayerName() << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printNegotiateOrder(NegotiateOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued a Negotiate order with " <<
                 pOrder->getTargetPlayer()->getPlayerName() << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a Negotiate order with " <<
                 pOrder->getTargetPlayer()->getPlayerName() << endl;
            break;
        default:
            break;
    }
}

GameState::GameState(int totalTerritories, vector<Player *> *players, Player *currentPlayer, Phase currentPhase,
                     const string &phaseInfo) : totalTerritories(totalTerritories), players(players),
                                                currentPlayer(currentPlayer), currentPhase(currentPhase),
                                                phaseInfo(phaseInfo) {}

GameState::GameState() {}

Player *GameState::getCurrentPlayer() const {
    return currentPlayer;
}

Phase GameState::getCurrentPhase() const {
    return currentPhase;
}

const string &GameState::getPhaseInfo() const {
    return phaseInfo;
}

const vector<Player *> *GameState::getPlayers() const {
    return players;
}

int GameState::getTotalTerritories() const {
    return totalTerritories;
}

void GameState::setPlayers(vector<Player *> *players) {
    GameState::players = players;
}

void GameState::setCurrentPlayer(Player *currentPlayer) {
    GameState::currentPlayer = currentPlayer;
}

void GameState::setCurrentPhase(Phase currentPhase) {
    GameState::currentPhase = currentPhase;
}

void GameState::updateGameState(Player *player, Phase phase) {
    setCurrentPhase(phase);
    setCurrentPlayer(player);
    notify();
}

//STATISTICS OBSERVER
void StatisticsObserver::update() {
    this->displayStatsUpdate();
}

StatisticsObserver::StatisticsObserver(GameState *currGameState) : currentGameState(currGameState) {}

StatisticsObserver::StatisticsObserver(const StatisticsObserver &original) {}

StatisticsObserver::~StatisticsObserver() {

}

StatisticsObserver &StatisticsObserver::operator=(const StatisticsObserver &otherObserver) {
    currentGameState = otherObserver.currentGameState;
    return *this;
}

void StatisticsObserver::displayStatsUpdate() {
    cout << '|' << "Player" << setw(3) << '|' << "Territorial Control" << setw(3) << '|' << endl;
    vector<float> playerDominationRatios{};
    for (Player *player: *currentGameState->getPlayers()) {
        float playerDomination = calculateWorldDomination(player->getTerritories().size());
        playerDominationRatios.push_back(playerDomination);
        cout << '|' << player->getPlayerName() << setw(3) << '|'
             << string("% ").append(to_string(playerDomination)) << setw(3) << '|' << endl;
    }
    for (int i = 0; i < playerDominationRatios.size(); i++) {
        if (playerDominationRatios[i] == 100.0) {
            cout << "~ CONGRATULATIONS " << currentGameState->getPlayers()->at(i)->getPlayerName()
                 << " YOU WON THE GAME! VICCCTORY ~" << endl;
        }
    }
}

float StatisticsObserver::calculateWorldDomination(int numberOfTerritories) {
    return (float) numberOfTerritories / (float) currentGameState->getTotalTerritories();
}


