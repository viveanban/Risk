#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"
#include "./../GameObservers/GameObservers.h"
/**
 * The GameEngine class is a Singleton and it represents the entire game
 */
class GameEngine {
private:
    vector<string> availableMaps;
    bool statisticsObserverActive;
    bool phaseObserverActive;
    int numPlayer;

    vector<Player *> players;
    Map *map;
    Deck *deck;
    GameState *gameState;

    GameEngine();

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

    void assignTerritoriesToPlayers();

    void assignArmiesToPlayers();

    bool winnerExists();

    void removePlayersWithoutTerritoriesOwned();

    int getBonus(Player *player);

    int calculateNumberOfArmiesToGive(Player *player);

    int getInitialArmyNumber();

public:

    static GameEngine *getInstance();

    GameEngine(const vector<string> &availableMaps, bool statisticsObserverActive, bool phaseObserverActive,
               int numPlayer, const vector<Player *> &players, Map *map, Deck *deck, GameState *gameState);

    static GameEngine *gameEngine;

    GameEngine(GameEngine &other) = delete;

    void operator=(const GameEngine &) = delete;

    ~GameEngine();

    int getNumPlayer() const;

    // responsible for initialization of the game
    void initializeGame();

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


    void startupPhase();

    /**
     * The mainGameLoop plays the game by going through the reinforcement phase, the issuing order phase and the executing order phase until there is a winner.
     */
    void mainGameLoop();

    void resetDiplomacy();

    bool isPhaseObserverActive() const;

    Deck *getDeck() const;

    Map *getMap() const;

    GameState *getGameState() const;

    const vector<Player *> &getPlayers() const;

    void setPhaseObserverActive(bool phaseObserverActive);

    void setGameState(GameState *gameState);

    void setPlayers(const vector<Player *> &players);

    void setMap(Map *map);


    void setDeck(Deck *deck);

    const vector<string> &getAvailableMaps() const;

    bool isStatisticsObserverActive() const;
};

#endif //RISK_GAMEENGINE_H
