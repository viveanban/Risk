#ifndef RISK_GAMEENGINE_H
#define RISK_GAMEENGINE_H


#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"
#include "./../GameObservers/GameObservers.h"
/**
 * The GameEngine class is a Singleton and it represents the entire game
 */
 //TODO: Suggestion: add a field MapType that will be set in the gameEngine (depending on which type to run, the correct map file reader will be called)
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

    enum MapType {conquest, domination};

    // Default constructor
    GameEngine();

    /**
     * SelectMap is responsible for displaying available maps from the ./maps folder
     * to the user and prompting him for a map
     */
    void selectMap();

    /**
     * SelectMapType is responsible for displaying available map types (i.e Conquest and Dominatoin)
     * to the user and prompting them for a map type
     */
    MapType selectMapType();

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

    // Randomizes the order of the players
    void randomlySetOrder();

    // Assigns territories to players in round robin fashion
    void assignTerritoriesToPlayers();

    // Assigns armies to each players based on the number of players
    void assignArmiesToPlayers();

    // Checks if a player has won the game.
    bool winnerExists();

    // Eliminates players that do not own any territories (have been conquered by other players)
    void removePlayersWithoutTerritoriesOwned();

    // Gets the number of armies each player should receive at the start of the game based on the number of players
    int getInitialArmyNumber();

public:
    // Returns the singleton instance of the GameEngine
    static GameEngine *getInstance();

    // Singleton Object
    static GameEngine *gameEngine;

    // Singleton does not allow for copy constructor (there must be only one instance)
    GameEngine(GameEngine &other) = delete;

    // Singleton does not allow for assignment operator (there must be only one instance)
    void operator=(const GameEngine &) = delete;

    ~GameEngine();

    // Returns the number of players in the game.
    int getNumPlayer() const;

    // Responsible for the initialization of the game.
    void initializeGame();

    // Responsible to initialize the set of players
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

    // Startup Phase of the game, randomizes player order, assigns territories and armies to players.
    void startupPhase();

    /**
     * The mainGameLoop plays the game by going through the reinforcement phase, the issuing order phase and the executing order phase until there is a winner.
     */
    void mainGameLoop();

    // Cancels the negotiations between players when the round has ended.
    void resetDiplomacy();

    // Checks if the Phase Observer flag was set at the start of the game.
    bool isPhaseObserverActive() const;

    // Checks if the Stats Observer flag was set at the start of the game.
    bool isStatisticsObserverActive() const;

    Deck *getDeck() const;

    Map *getMap() const;

    GameState *getGameState() const;

    const vector<Player *> &getPlayers() const;

    // Calculates the total number of armies including bonus to give during reinforcement phase.
    int calculateNumberOfArmiesToGive(Player *player);

    // Calculates the bonus to give during reinforcement phase.
    int getBonus(Player *player);
};

#endif //RISK_GAMEENGINE_H
