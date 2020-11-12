//
// Created by tarek ait hamouda on 2020-11-12.
//

#ifndef RISK_GAMEOBSERVERS_H
#define RISK_GAMEOBSERVERS_H

#include "../Player/Player.h"

/**
 * Part 5: Observers
Provide a group of C++ classes that implements observers who are used to display information to the
 user as the game is being played. The user shall have the option to turn on/off any of the two
 observers during the game start phase (see Part 1).
Phase Observer
Using the Observer design pattern, implement a view that displays information happening in the current
 phase. It should first display a header showing what player and what phase is currently being played,
 e.g. “Player 3: Reinforcement phase” or “Player 1: Issue orders phase” Then it should display
 important information related to what is happening in this phase, which should be different depending
 on what phase is being played. This should dynamically be updated as the game goes through different
 players/phases and be visible at all times during
5
game play. You must deliver a driver that demonstrates that (1) the information displayed by the phase
 view is cleared every time the phase is changing (2) the phase view is displaying the correct
 player:phase information as soon as the phase changes; (3) the phase view displays relevant
 information which is different for every phase. The Observer and Observable classes code must be
 implemented in a new GameObservers.cpp/GameObservers.h file.
Game Statistics Observer
Using the Observer design pattern, implement a view that displays some useful statistics about the
 game, the minimum being a “players world domination view” that shows using some kind of graph or
 table depicting what percentage of the world is currently being controlled by each player. This
 should dynamically be updated as the map state changes and be visible at all times during game play.
 You must deliver a driver that demonstrates that (1) the game statistics view updates itself every
 time a country has been conquered by a player; (2) the game statistics updates itself when a player
 has been eliminated and removes this player from the view; (3) as soon as a player owns all the
 countries, the game statistics view updates itself and displays a celebratory message. The Observer
 and Observable classes code must be implemented in a new GameObservers.cpp/GameObservers.h file duo.
 */

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
