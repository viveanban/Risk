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
    bool phaseObserver = false;
    bool statisticsObserver = false;
    int numPlayer;
    void selectMap();
    void selectPlayerNumber();
    void setAvailableMaps(const string &path);
    void setupObservers();

public:
    Map *getMap() const;

    void setMap(Map *map);

    Deck *getDeck() const;

    void setDeck(Deck *deck);

    const vector<Player *> &getPlayers() const;

    void setPlayers(const vector<Player *> &players);

    const vector<string> &getAvailableMaps() const;

    void setAvailableMaps1(const vector<string> &availableMaps);

    bool isPhaseObserver() const;

    void setPhaseObserver(bool phaseObserver);

    bool isStatisticsObserver() const;

    void setStatisticsObserver(bool statisticsObserver);

    int getNumPlayer() const;

    void setNumPlayer(int numPlayer);

public:
    void gameStart();


    void setupPlayers();

    void setupOrders();

    void assignCards();
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
