#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"

class GameInitialization {
private:
    Map *map;
    Deck *deck;
    vector<Player *> players;
    vector<string> availableMaps;
    bool phaseObserver = false;
    bool statisticsObserver = false;
    int numPlayer;

    void selectMap();

    void selectPlayerNumber();

    void setAvailableMaps(const char *path);

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

    void gameStart();

    void setupPlayers();

    void setupOrders();

    void assignCards();

    int openMapFile(const string &MAP_DIRECTORY, int chosenMap, ifstream &inputFile) const;

    int isRegularFile(const char *path);

    int validateNumberPlayerInput(int numPlayerTmp) const;
};

class GameSetup {
private:
    vector<Player *> listOfPlayers;
    Map *map;
    void randomlySetOrder();
    void assignCountriesToPlayers();
    void assignArmiesToPlayers();

public:
    GameSetup(vector<Player *> players, Map *map);

    void startupPhase();
    int getInitialArmyNumber();
};

#endif //RISK_GAMEENGINE_H
