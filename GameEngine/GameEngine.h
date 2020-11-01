#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"

class GameSetup {
private:
    Map* map;
    Deck* deck;
    vector<Player*> players;
    vector<string> availableMaps;
    void selectMap();
    void selectPlayerNumber();
public:
    void gameStart();

    void setAvailableMaps(const string &path);
};

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

    GameEngine(Map *map, Deck *deck, vector<Player *> &players);

    ~GameEngine();
    void startupPhase();
    void mainGameLoop();
};


#endif //RISK_GAMEENGINE_H
