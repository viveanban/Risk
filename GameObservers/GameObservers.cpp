//
// Created by tarek ait hamouda on 2020-11-12.
//

#include "GameObservers.h"


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
}

void PhaseObserver::update() {

    if (currentGameState->getCurrentPlayer() == nullptr || currentGameState->getCurrentPhase() == NULL) {
        cout << "ERROR OCCURED WHILE TRYING TO UPDATE PHASE OBSERVER. NULL VALUE PASSED" << endl;
        return;
    }
    displayPhaseUpdates();
}

void PhaseObserver::displayPhaseUpdates() {
    cout << currentGameState->getCurrentPlayer()->getPlayerName() << " " << getPhaseText() << endl;
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
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " received "
                 << currentGameState->getCurrentPlayer()->getNumberOfArmies() << " number of armies." << endl;
            break;
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued an order of type: "
                 << currentGameState->getCurrentPlayer()->getOrders()->getOrderList().back()->description << endl;
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " has "
                 << currentGameState->getCurrentPlayer()->getNumberOfArmies() << " number of armies left to deploy." << endl;
            break;
        case orders_execution:
            cout << currentGameState->getPhaseInfo() << endl;
            break;
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
//    Observer::update();
}

StatisticsObserver::StatisticsObserver(GameState *currGameState) : currGameState(currGameState) {}

StatisticsObserver::StatisticsObserver(const StatisticsObserver &original) {}

StatisticsObserver::~StatisticsObserver() {}

StatisticsObserver &StatisticsObserver::operator=(const StatisticsObserver &otherObserver) {

}



