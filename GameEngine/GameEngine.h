#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"

class GameEngine {
private:
    Map* map;
    Deck* deck;
    vector<Player*> players;

    /**
     * The reinforcementPhase method determines how many armies to give to a player
     * @param player: a pointer to a Player object
     * @return void.
     */
    void reinforcementPhase();

    /**
     * The issueOrdersPhase method ____________
     * @param
     * @return
     */
    void issueOrdersPhase();

    /**
     * The executeOrdersPhase method _______________
     * @param
     * @return
     */
    void executeOrdersPhase();

    bool winnerExists();

public:
    GameEngine();
    ~GameEngine();
    void startupPhase();
    void mainGameLoop();
};


#endif //RISK_GAMEENGINE_H
