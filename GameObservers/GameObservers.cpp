//
// Created by tarek ait hamouda on 2020-11-12.
//

#include "GameObservers.h"

//PHASE OBSERVER

PhaseObserver::PhaseObserver(GameState *currGameState) : currGameState(currGameState) {}

PhaseObserver::~PhaseObserver() {
//TODO: implement destructor}
}

PhaseObserver::PhaseObserver(const PhaseObserver &original) {
//TODO: implement copy constructor
}

PhaseObserver &PhaseObserver::operator=(const PhaseObserver &otherObserver) {
//TODO: implement operator
}

void PhaseObserver::update() {

    if (currGameState->getCurrentPlayer() == nullptr || currGameState->getCurrentPhase() == NULL) {
        cout << "ERROR OCCURED WHILE TRYING TO UPDATE PHASE OBSERVER. NULL VALUE PASSED" << endl;
        return;
    }
    displayPhaseUpdates();
}

void PhaseObserver::displayPhaseUpdates() {
    cout << currGameState->getCurrentPlayer()->getPlayerName() << " " << getPhaseText() << endl;
    cout << currGameState->getPhaseInfo() << endl;
}

string PhaseObserver::getPhaseText() {
    switch (currGameState->getCurrentPhase()) {
        case reinforcement:
            return "Reinforcement";
        case issuing_orders:
            return "Issue orders";
        case orders_execution:
            return "Orders execution";
    }
}


Player *GameState::getCurrentPlayer() const {
    return currentPlayer;
}

Phase GameState::getCurrentPhase() const {
    return currentPhase;
}

const string &GameState::getPhaseInfo() const {
    return phaseInfo;
}

//STATISTICS OBSERVER

void StatisticsObserver::update() {
    Observer::update();
}

StatisticsObserver::StatisticsObserver(GameState *currGameState) : currGameState(currGameState) {}

StatisticsObserver::StatisticsObserver(const StatisticsObserver &original) {}

StatisticsObserver::~StatisticsObserver() {}

StatisticsObserver &StatisticsObserver::operator=(const StatisticsObserver &otherObserver) {

}

