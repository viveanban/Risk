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
private:
    list<Observer *> *observers;

public:
    virtual void attach(Observer *o);

    virtual void detach(Observer *o); // TODO: add comment why we are nt using it (Ferdou)

    virtual void notify();

    Subject();

    ~Subject();

    list<Observer *> *getObservers() const;
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

    ~GameState();

    GameState(int totalTerritories, Player *currentPlayer, Phase currentPhase);

    GameState(const GameState &original);

    GameState &operator=(const GameState &original);

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
    virtual ~Observer();
};

class StatisticsObserver : public Observer {
private:
    GameState *currentGameState;

    void displayStatsUpdate();

    float calculateWorldDomination(int numberOfTerritories);

public:
    StatisticsObserver();

    explicit StatisticsObserver(GameState *currGameState);

    ~StatisticsObserver();

    StatisticsObserver(const StatisticsObserver &original);

    StatisticsObserver &operator=(const StatisticsObserver &otherObserver);

    void update() override;
};

class PhaseObserver : public Observer {
public:
    PhaseObserver();

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

    void printOrderInfo(Order *order, Card *card);

    void printBombOrder(BombOrder *pOrder);

    void printAdvanceOrder(AdvanceOrder *pOrder);

    void printDeployOrderInfo(DeployOrder *pOrder);

    void printBlockadeOrder(BlockadeOrder *pOrder);

    void printAirliftOrder(AirliftOrder *pOrder);

    void printNegotiateOrder(NegotiateOrder *pOrder);

    void printReinforcementinfo() const;

    void printReinforcementCardInfo();

    void printToDefendToAttack();
};

#endif //RISK_GAMEOBSERVERS_H
