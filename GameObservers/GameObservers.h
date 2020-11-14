//
// Created by tarek ait hamouda on 2020-11-12.
//

#ifndef RISK_GAMEOBSERVERS_H
#define RISK_GAMEOBSERVERS_H

#include <iostream>
#include <list>
#include "../Player/Player.h"

enum Phase {
    reinforcement, issuing_orders, orders_execution
};

// Forward reference
class Observer;

class Subject {
public:
    virtual void attach(Observer *o);

    virtual void detach(Observer *o);

    virtual void notify();

    Subject();

    ~Subject();

private:
    list<Observer *> *observers;
};

class GameState : public Subject {
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
    virtual void update() = 0;
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
    GameState *currentGameState;

    void displayPhaseUpdates();

    string getPhaseText();

    void displaySpecialInformation();

    void printOrderInfo(Order *order);

    void printBombOrder(BombOrder *pOrder);

    void printAdvanceOrder(AdvanceOrder *pOrder);

    void printDeployOrderInfo(DeployOrder *pOrder);

    void printBlockadeOrder(BlockadeOrder *pOrder);

    void printAirliftOrder(AirliftOrder *pOrder);

    void printNegotiateOrder(NegotiateOrder *pOrder);

    void printReinforcementinfo() const;
};

#endif //RISK_GAMEOBSERVERS_H
