//
// Created by tarek ait hamouda on 2020-11-12.
//

#ifndef RISK_GAMEOBSERVERS_H
#define RISK_GAMEOBSERVERS_H

#include <iostream>;
#include "../Player/Player.h"

enum Phase {
    reinforcement, issuing_orders, orders_execution
};

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

class StatisticsObserver : Observer {

public:
    explicit StatisticsObserver(GameState *currGameState);

    ~StatisticsObserver();

    StatisticsObserver(const StatisticsObserver &original);

    StatisticsObserver &operator=(const StatisticsObserver &otherObserver);

    void update() override;

private:
    GameState *currGameState;

};

class PhaseObserver : Observer {
public:
    explicit PhaseObserver(GameState *currGameState);

    ~PhaseObserver();

    PhaseObserver(const PhaseObserver &original);

    PhaseObserver &operator=(const PhaseObserver &otherObserver);

    void update() override;

private:
    GameState *currGameState;

    void displayPhaseUpdates();

    string getPhaseText();
};

#endif //RISK_GAMEOBSERVERS_H
