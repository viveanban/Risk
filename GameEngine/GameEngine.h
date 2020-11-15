#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"

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

    static bool getTrueFalseInputFromUser(string resultName);

public:

    Map *getMap() const;

    Deck *getDeck() const;

    const vector<Player *> &getPlayers() const;

    const vector<string> &getAvailableMaps() const;

    bool isPhaseObserver() const;

    bool isStatisticsObserver() const;

    int getNumPlayer() const;

    void gameStart(); // TODO: rename

    void setupPlayers();

    int openMapFile(const string &MAP_DIRECTORY, int chosenMap, ifstream &inputFile) const;

    static int isRegularFile(const char *path);

    static int validateNumberPlayerInput(int numPlayerTmp);
};

/**
 * The GameEngine class is a Singleton and it represents the entire game
 */
class GameEngine {
private:
    vector<Player *> players;
    Map *map;
    Deck *deck;

    GameEngine();

    /**
     * The reinforcementPhase determines how many armies to give to a player
     */
    void reinforcementPhase();

    /**
     * The issueOrdersPhase allows players to issue their orders in a round-robin fashion
     */
    void issueOrdersPhase();

    /**
     * The executeOrdersPhase executes each player's orders in a round-robin fashion
     */
    void executeOrdersPhase();

    void randomlySetOrder();

    void assignCountriesToPlayers();

    void assignArmiesToPlayers();

    bool winnerExists();

    void removePlayersWithoutTerritoriesOwned();

    int getBonus(Player *player);

    int calculateNumberOfArmiesToGive(Player *player);

    int getInitialArmyNumber();

public:
    static GameEngine* gameEngine;

    GameEngine(GameEngine &other) = delete;

    void operator=(const GameEngine &) = delete;

    static GameEngine* getInstance();

    ~GameEngine();

    void startupPhase();

    /**
     * The mainGameLoop plays the game by going through the reinforcement phase, the issuing order phase and the executing order phase until there is a winner.
     */
    void mainGameLoop();

    const vector<Player *> &getPlayers() const;

    void setPlayers(const vector<Player *> &players);

    Map *getMap() const;

    void setMap(Map *map);

    Deck *getDeck() const;

    void setDeck(Deck *deck);
};

#endif //RISK_GAMEENGINE_H
