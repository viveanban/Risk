#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"
#include "./../GameObservers/GameObservers.h"

class GameInitialization {
private:
    Map *map;
    Deck *deck;
    vector<Player *> players;
    vector<string> availableMaps;
    bool phaseObserver = false;
    bool statisticsObserver = false;
    GameState *gameState;
    int numPlayer;
    /**
     * SelectMap is responsible for displaying available maps from the ./maps folder
     * to the user and prompting him for a map
     */
    void selectMap();

    /**
     * selectPlayerNumber is responsible for prompting the user on how many players he wants
     */
    void selectPlayerNumber();

    /**
     * setAvailableMaps is responsible for fetching all available maps to be chosen from later
     */
    void setAvailableMaps(const char *path);

    /**
     * setupObservers is responsible for setting up all the observers for the game
     */
    void setupObservers();

    /**
     * getTrueFalseInputFromUser is responsible for prompting a user for true false answer while returning the result
     */
    static bool getTrueFalseInputFromUser(string resultName);

public:

    //GETTERS
    GameInitialization();
    Map *getMap() const;

    Deck *getDeck() const;

    const vector<Player *> &getPlayers() const;

    const vector<string> &getAvailableMaps() const;

    bool isPhaseObserver() const;

    bool isStatisticsObserver() const;

    int getNumPlayer() const;

    // responsible for initialization of the game
    void gameStart();

    // responsible to initialize the set of players
    void setupPlayers();

    /**
     * openMapFile responsible for opening up and returning a given mapFile from the user's choice
     * @param MAP_DIRECTORY map directory where map files reside
     * @param chosenMap chosen map that the user wants to use
     * @param inputFile
     * @return the index of the opened file
     */
    int openMapFile(const string &MAP_DIRECTORY, int chosenMap, ifstream &inputFile) const;

    // makes sure the file is not a directory or unknown format
    static int isRegularFile(const char *path);

    /**
     * validateNumberPlayerInput as its name indicates validate the player number that it receives and return an accepted one
     * @param numPlayerTmp : initial number that the user provides
     * @return a verified and valid number of player
     */
    static int validateNumberPlayerInput(int numPlayerTmp);
};

class GameEngine {
private:
    vector<Player *> players;
    Map *map;
    Deck *deck;
    GameState *gameState;

    void randomlySetOrder();

    void assignCountriesToPlayers();

    void assignArmiesToPlayers();
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

    int getBonus(Player *player);

public:
    GameEngine(vector<Player *> players, Map *map, Deck *deck, GameState *gameState);

    ~GameEngine();

    void startupPhase();

    int getInitialArmyNumber();

    void mainGameLoop();

    int calculateNumberOfArmiesToGive(Player *player);

    void updateGameState(Player *pPlayer, Phase phase);
};

#endif //RISK_GAMEENGINE_H
