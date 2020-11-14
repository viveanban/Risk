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
                 << currentGameState->getCurrentPlayer()->getNumberOfArmies() << " number of armies left to deploy."
                 << endl;
            break;
        case orders_execution:
            cout << currentGameState->getPhaseInfo() << endl;
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

//STATISTICS OBSERVER
void StatisticsObserver::update() {
    this->displayStatsUpdate();
}

StatisticsObserver::StatisticsObserver(GameState *currGameState) : currGameState(currGameState) {}

StatisticsObserver::StatisticsObserver(const StatisticsObserver &original) {}

StatisticsObserver::~StatisticsObserver() {

}

StatisticsObserver &StatisticsObserver::operator=(const StatisticsObserver &otherObserver) {
    this->currGameState = otherObserver.currGameState;
}

void StatisticsObserver::displayStatsUpdate() {
    cout << '|' << "Player" << setw(3) << '|' << "Territorial Control" << setw(3) << '|' << endl;
    vector<float> playerDominationRatios{};
    for (Player *player: *currGameState->getPlayers()) {
        float playerDomination = calculateWorldDomination(player->getTerritories().size());
        playerDominationRatios.push_back(playerDomination);
        cout << '|' << player->getPlayerName() << setw(3) << '|'
             << string("% ").append(to_string(playerDomination)) << setw(3) << '|' << endl;
    }
    for (int i = 0; i < playerDominationRatios.size(); i++) {
        if (playerDominationRatios[i] == 100.0) {
            cout << "~ CONGRATULATIONS " << currGameState->getPlayers()->at(i)->getPlayerName()
                 << " YOU WON THE GAME! VICCCTORY ~" << endl;
        }
    }
}

float StatisticsObserver::calculateWorldDomination(int numberOfTerritories) {
    return (float) numberOfTerritories / (float) currGameState->getTotalTerritories();
}


