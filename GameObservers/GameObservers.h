//
// Created by tarek ait hamouda on 2020-11-12.
//

#ifndef RISK_GAMEOBSERVERS_H
#define RISK_GAMEOBSERVERS_H
#include <iostream>;
#include "../Player/Player.h"

class StatisticsObserver {

public:
    StatisticsObserver();

    ~StatisticsObserver();

    StatisticsObserver(const StatisticsObserver &original);

    void operator=(const StatisticsObserver &otherObserver);

private:

};
enum Phase { reinforcement, issuing_orders, orders_execution };
class PhaseObserver {
public:
    PhaseObserver();

    ~PhaseObserver();

    PhaseObserver(const PhaseObserver &original);

    void operator=(const PhaseObserver &otherObserver);

    void update(Player* player, Phase phase);
private:
    Player* currentPlayer;
    Phase currentPhase;
    void displayPhaseUpdates();
    string getPhaseText();
};


#endif //RISK_GAMEOBSERVERS_H
