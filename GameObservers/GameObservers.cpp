//
// Created by tarek ait hamouda on 2020-11-12.
//

#include "GameObservers.h"

PhaseObserver::PhaseObserver() {
//TODO: implement constructor
}

PhaseObserver::~PhaseObserver() {
//TODO: implement destructor}
}

PhaseObserver::PhaseObserver(const PhaseObserver &original) {
//TODO: implement copy constructor
}

void PhaseObserver::operator=(const PhaseObserver &otherObserver) {
//TODO: implement operator
}

void PhaseObserver::update(Player *player, Phase phase) {

    if (player == nullptr || phase == NULL) {
        cout << "ERROR OCCURED WHILE TRYING TO UPDATE PHASE OBSERVER. NULL VALUE PASSED" << endl;
        return;
    }
    this->currentPlayer = player;
    this->currentPhase = phase;

    displayPhaseUpdates();
}

void PhaseObserver::displayPhaseUpdates() {
    cout << currentPlayer->getPlayerName() << " " << getPhaseText() << endl;
    //TODO: display important information related to what is happening in this phase
}

string PhaseObserver::getPhaseText() {
    switch (currentPhase) {
        case reinforcement:
            return "Reinforcement";
        case issuing_orders:
            return "Issue orders";
        case orders_execution:
            return "Orders execution";
    }
}


