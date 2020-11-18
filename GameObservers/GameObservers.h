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
    Order *currentOrder;
    Card *currentCard;
    Phase currentPhase;
    int totalTerritories;
public:
    GameState();

    GameState(int totalTerritories, Player *currentPlayer, Phase currentPhase);

    GameState(const GameState &original);

    GameState &operator=(const GameState &original);

    virtual ~GameState();

    void updateGameState(Player *player, Phase phase, Order *order, Card *card);

    void setCurrentPlayer(Player *currentPlayer);

    void setCurrentPhase(Phase currentPhase);

    Player *getCurrentPlayer() const;

    Phase getCurrentPhase() const;

    int getTotalTerritories() const;

    Order *getCurrentOrder() const;

    void setCurrentOrder(Order *currentOrder);

    Card *getCurrentCard() const;

    void setCurrentCard(Card *currentCard);

    void setTotalTerritories(int totalTerritories);
};

class Observer {
public:
    virtual void update() = 0;
};

class StatisticsObserver : public Observer {
public:
    StatisticsObserver();

    StatisticsObserver(GameState *currGameState);

    ~StatisticsObserver();

    StatisticsObserver(const StatisticsObserver &original);

    StatisticsObserver &operator=(const StatisticsObserver &otherObserver);

    void update() override;

private:
    GameState *currentGameState;

    void displayStatsUpdate();

    float calculateWorldDomination(int numberOfTerritories);
};

class PhaseObserver : public Observer {
public:
    PhaseObserver();

    PhaseObserver(GameState *currGameState);

    ~PhaseObserver();

    PhaseObserver(const PhaseObserver &original);

    PhaseObserver &operator=(const PhaseObserver &otherObserver);

    void update() override;

private:
    GameState *currentGameState;

    void displayPhaseUpdates();

    string getPhaseText();

    void displaySpecialInformation();

    void printOrderInfo(Order *order, Card *card);

    void printBombOrder(BombOrder *pOrder);

    void printAdvanceOrder(AdvanceOrder *pOrder);

    void printDeployOrderInfo(DeployOrder *pOrder);

    void printBlockadeOrder(BlockadeOrder *pOrder);

    void printAirliftOrder(AirliftOrder *pOrder);

    void printNegotiateOrder(NegotiateOrder *pOrder);

    void printReinforcementinfo() const;

    void printReinforcementCardInfo();
};

#endif //RISK_GAMEOBSERVERS_H
