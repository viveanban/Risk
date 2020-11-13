//
// Created by tarek ait hamouda on 2020-11-12.
//

#ifndef RISK_GAMEOBSERVERS_H
#define RISK_GAMEOBSERVERS_H

#include <iostream>;
#include "../Player/Player.h"
class GameState {
private:
    Player *currentPlayer;
    Phase currentPhase;
    string phaseInfo;
public:
    Player *getCurrentPlayer() const;

    Phase getCurrentPhase() const;

    const string &getPhaseInfo() const;
};

class Observer {
public:
    virtual void update();
};

class StatisticsObserver {

public:
    StatisticsObserver();

    ~StatisticsObserver();

    StatisticsObserver(const StatisticsObserver &original);

    void operator=(const StatisticsObserver &otherObserver);

    void update();

private:

};

enum Phase {
    reinforcement, issuing_orders, orders_execution
};

class PhaseObserver {
public:
    PhaseObserver();

    ~PhaseObserver();

    PhaseObserver(const PhaseObserver &original);

    void operator=(const PhaseObserver &otherObserver);

    void update();

private:
    GameState* currGameState;

    void displayPhaseUpdates();

    string getPhaseText();
};
#endif //RISK_GAMEOBSERVERS_H
