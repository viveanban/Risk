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

    void gameStart();

    void setupPlayers();

    int openMapFile(const string &MAP_DIRECTORY, int chosenMap, ifstream &inputFile) const;

    static int isRegularFile(const char *path);

    static int validateNumberPlayerInput(int numPlayerTmp);
};

class GameSetup {
private:
    vector<Player *> listOfPlayers;
    Map *map;
    Deck *deck;

    void randomlySetOrder();

    void assignCountriesToPlayers();

    void assignArmiesToPlayers();

public:
    GameSetup(vector<Player *> players, Map *map, Deck *deck);

    void startupPhase();

    int getInitialArmyNumber();
};

//TODO: Merge GameEngine with GameSetup
class GameEngine {
private:
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

    void removePlayersWithoutTerritoriesOwned();

public:
    GameEngine();

    ~GameEngine();

    void mainGameLoop();

    int calculateNumberOfArmiesToGive(Player *player);
};

#endif //RISK_GAMEENGINE_H
