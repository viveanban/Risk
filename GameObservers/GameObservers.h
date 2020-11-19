#ifndef RISK_GAMEOBSERVERS_H
#define RISK_GAMEOBSERVERS_H

#include <iostream>
#include <list>
#include "../Player/Player.h"

// Enum to pass the current phase the game engine is in.
enum Phase {
    reinforcement, issuing_orders, orders_execution
};

// Forward reference.
class Observer;

// The subject is the class issuing notifications to the observer.
class Subject {
private:
    list<Observer *> *observers;

public:
    virtual void attach(Observer *o);

    // This method is not used because we never allow the player to detach the observers
    // from the subject after the game has been initialized.
    virtual void detach(Observer *o);

    // Notifies all the observers of changes.
    virtual void notify();

    // Default constructor.
    Subject();

    // Destructor.
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
    // Default Constructor.
    GameState();

    // Destructor.
    ~GameState();

    // Parameterized Constructor.
    GameState(int totalTerritories, Player *currentPlayer, Phase currentPhase);

    // Copy constructor.
    GameState(const GameState &original);

    // Assignment Operator.
    GameState &operator=(const GameState &original);

    // Update the game state and send a notification to the observers.
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
    // Update the observer to change its values and display new results.
    // Pure virtual method must be implemented by the children.
    virtual void update() = 0;

    virtual ~Observer();
};

class StatisticsObserver : public Observer {
private:
    GameState *currentGameState;

    // Print out the table of player world domination.
    void displayStatsUpdate();

    // Calculate the portion of the territories that is controlled by a player.
    float calculateWorldDomination(int numberOfTerritories);

public:
    StatisticsObserver();

    explicit StatisticsObserver(GameState *currGameState);

    // Copy Constructor.
    StatisticsObserver(const StatisticsObserver &original);

    // Assignment Operator.
    StatisticsObserver &operator=(const StatisticsObserver &otherObserver);

    // Implementation of the virtual method of the parent class, displays the table of world domination.
    void update() override;
};

class PhaseObserver : public Observer {
public:
    PhaseObserver();

    explicit PhaseObserver(GameState *currGameState);

    // Copy Constructor.
    PhaseObserver(const PhaseObserver &original);

    // Assignment Operator.
    PhaseObserver &operator=(const PhaseObserver &otherObserver);

    // Implementation of the virtual method of the parent class, displays the phase updates.
    void update() override;

private:
    GameState *currentGameState;

    // Prints the phase updates.
    void displayPhaseUpdates();

    // Get the description of the phase being played.
    string getPhaseText();

    // Prints additional information for the reinforcement phase and order execution.
    void displaySpecialInformation();

    // Prints the info for the order.
    void printOrderInfo(Order *order, Card *card);

    // Prints the info for Bomb Order.
    void printBombOrder(BombOrder *pOrder);

    // Prints the info for the Advance Order.
    void printAdvanceOrder(AdvanceOrder *pOrder);

    // Prints the info for the Deploy Order.
    void printDeployOrderInfo(DeployOrder *pOrder);

    // Prints the info for the Blockade Order.
    void printBlockadeOrder(BlockadeOrder *pOrder);

    // Prints the info for the Airlift Order.
    void printAirliftOrder(AirliftOrder *pOrder);

    // Prints the info for the Negotiate Order.
    void printNegotiateOrder(NegotiateOrder *pOrder);

    // Prints information regarding the reinforcements.
    void printReinforcementInfo() const;

    // Prints the information of the reinforcement card.
    void printReinforcementCardInfo();

    // Prints the list of territories the player can defend and attack.
    void printToDefendToAttack();
};

#endif //RISK_GAMEOBSERVERS_H
