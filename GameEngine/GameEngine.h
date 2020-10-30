#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"

class GameEngine {
private:
    Map* map;
    Deck* deck;
    vector<Player*> players;

    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();

public:
    GameEngine();
    ~GameEngine();
    void startupPhase();
    void mainGameLoop();
};


#endif //RISK_GAMEENGINE_H
