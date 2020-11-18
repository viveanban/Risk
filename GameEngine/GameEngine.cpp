#include <fstream>
#include "GameEngine.h"
#include <set>
#include "../MapLoader/MapLoader.h"
#include "./../GameObservers/GameObservers.h"
#include <random>
#include <string>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// ---------GAME INITIALIZATION---------------
void GameInitialization::initializeGame() {
    selectMap();
    selectPlayerNumber();
    setupObservers();
    setupPlayers();
    this->deck = new Deck(50); // TODO: print out that 50 cards are created and show that there's every type of card in the deck (Tarek)
    gameState->setTotalTerritories(map->getTerritoryList().size());
}

void GameInitialization::selectMap() {
    const string MAP_DIRECTORY = "../maps/";
    int chosenMap;
    ifstream inputFile;
    do {
        cout << "Please enter the number of the game Map you wish to play from the following list:" << endl;
        auto path = "../maps/";
        setAvailableMaps(path);
        for (int i = 1; i <= availableMaps.size(); i++) {
            cout << i << " - " << availableMaps.at(i - 1) << endl;
        }
        chosenMap = openMapFile(MAP_DIRECTORY, chosenMap, inputFile);
        while (!inputFile.is_open()) {
            inputFile.close();
            cout << "Hey you made a mistake, " << chosenMap << " is not one of the choices." << endl;
            cout << "Please pick another map now: " << endl;
            chosenMap = openMapFile(MAP_DIRECTORY, chosenMap, inputFile);
        }
        this->map = MapLoader::loadMap(availableMaps.at(chosenMap - 1));
    } while (map == NULL or !map->validate());
    inputFile.close();
}

int GameInitialization::openMapFile(const string &MAP_DIRECTORY, int chosenMap, ifstream &inputFile) const {
    cin >> chosenMap;
    if (cin.fail()) {
        cin.clear();
        chosenMap = -1;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (chosenMap > 0 and chosenMap < availableMaps.size()) {
        inputFile.open(MAP_DIRECTORY + availableMaps.at(chosenMap - 1));
    }
    return chosenMap;
}

void GameInitialization::setAvailableMaps(const char *path) {
    DIR *dir = opendir(path);
    struct dirent *current;
    this->availableMaps.clear();
    if (dir != nullptr) {
        string parent(path);
        while ((current = readdir(dir)) != nullptr) {
            string filePath = parent + current->d_name;
            const char *entry = filePath.c_str();
            if (isRegularFile(entry)) {
                this->availableMaps.push_back(current->d_name);
            }
        }
        closedir(dir);
    }
}

int GameInitialization::isRegularFile(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void GameInitialization::selectPlayerNumber() {
    int numPlayerTmp = -1;
    int maxPlayerNumber = std::min( (int)map->getTerritoryList().size(), 5);
    cout << "The game supports up to "<< maxPlayerNumber <<" players with a minimum of 2."
            " Please input the desired number of players" << endl;
    numPlayerTmp = validateNumberPlayerInput(numPlayerTmp);
    while (numPlayerTmp < 2 or numPlayerTmp > maxPlayerNumber) {
        cout << "This does not look like a number between 2 to " << maxPlayerNumber <<
                ". The game supports up to 5 players with a minimum of 2." << endl <<
             "Please input the desired number of players" << endl;
        numPlayerTmp = validateNumberPlayerInput(numPlayerTmp);
    }
    this->numPlayer = numPlayerTmp;
}

int GameInitialization::validateNumberPlayerInput(int numPlayerTmp) {
    cin >> numPlayerTmp;
    if (cin.fail()) {
        cin.clear();
        numPlayerTmp = 0;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return numPlayerTmp;
}

void GameInitialization::setupObservers() {
    if (getTrueFalseInputFromUser("phase")) {
        gameState->attach(new PhaseObserver(gameState));
        phaseObserver = true;
    } else {
        phaseObserver = false;
    }

    if (getTrueFalseInputFromUser("statistics"))
        gameState->attach(new StatisticsObserver(gameState));

}

bool GameInitialization::getTrueFalseInputFromUser(string resultName) {
    bool result = false;
    do {
        cout << "Do you want to turn on the " << resultName << " observer [true/false]" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> boolalpha >> result;
    } while (cin.fail());
    return result;
}

void GameInitialization::setupPlayers() {

    for (int i = 0; i < this->getNumPlayer(); i++) {
        this->players.push_back(new Player("Player " + to_string(i + 1)));
    }
}

Map *GameInitialization::getMap() const {
    return map;
}

Deck *GameInitialization::getDeck() const {
    return deck;
}

const vector<Player *> &GameInitialization::getPlayers() const {
    return players;
}

const vector<string> &GameInitialization::getAvailableMaps() const {
    return availableMaps;
}

bool GameInitialization::isPhaseObserver() const {
    return phaseObserver;
}

bool GameInitialization::isStatisticsObserver() const {
    return statisticsObserver;
}

int GameInitialization::getNumPlayer() const {
    return numPlayer;
}

GameInitialization::GameInitialization() : map(nullptr), deck(nullptr), numPlayer(0),
                                           gameState(new GameState(0, nullptr, reinforcement)) {}

GameInitialization::~GameInitialization() {
    // TODO: we need to delete observers (Tarek)

    for (auto player: GameEngine::getInstance()->getPlayers()) { // TODO: (merge everything together) (Abhijit)
        delete player;
        player = nullptr;
    }
    players.clear();
    cout << "Deleted all remaining Players" << endl;

    delete Player::neutralPlayer;
    Player::neutralPlayer = nullptr;
    cout << "Deleted the Neutral Player" << endl;

    delete gameState;
    gameState = nullptr;
    cout << "Deleted Gamestate" << endl;

    delete deck;
    deck = nullptr;
    cout << "Deleted Deck" << endl;

    delete map;
    map = nullptr;
    cout << "Deleted Map" << endl;
}

GameInitialization &GameInitialization::operator=(const GameInitialization &otherGameInitialization) {
    map = otherGameInitialization.getMap();
    players = otherGameInitialization.getPlayers();
    deck = otherGameInitialization.getDeck();
    gameState = otherGameInitialization.getGameState();
    phaseObserver = otherGameInitialization.isPhaseObserver();
    statisticsObserver = otherGameInitialization.isStatisticsObserver();
    numPlayer = otherGameInitialization.getNumPlayer();
    return *this;
}

GameInitialization::GameInitialization(GameInitialization &original) {
    map = original.getMap();
    players = original.getPlayers();
    deck = original.getDeck();
    gameState = original.getGameState();
    phaseObserver = original.isPhaseObserver();
    statisticsObserver = original.isStatisticsObserver();
    numPlayer = original.getNumPlayer();
}

std::ostream &operator<<(ostream &stream, GameInitialization &gameInitialization) {
    return stream << "Information on GameInitialization object:" << endl
                  << "map: " << gameInitialization.map << endl
                  << "Number of players: " << gameInitialization.players.size() << endl
                  << "Statistic Observer on: " << boolalpha << gameInitialization.isStatisticsObserver() << endl
                  << "Phase Observer on: " << boolalpha << gameInitialization.isPhaseObserver() << endl
                  << "Deck: " << gameInitialization.getDeck() << endl;
}

GameState *GameInitialization::getGameState() const {
    return gameState;
}

//GAME STARTUP PHASE
// ---------GAME ENGINE---------------
GameEngine *GameEngine::gameEngine = nullptr;

GameEngine::GameEngine()
        : players(), map(nullptr), deck(nullptr), gameState{nullptr}, gameInitialization(nullptr),
          phaseObserverActive{} {}

GameEngine *GameEngine::getInstance() {
    if (gameEngine == nullptr) {
        gameEngine = new GameEngine();
        gameEngine->gameInitialization = new GameInitialization();
        gameEngine->gameInitialization->initializeGame();
        gameEngine->deck = gameEngine->gameInitialization->getDeck();
        gameEngine->map = gameEngine->gameInitialization->getMap();
        gameEngine->players = gameEngine->gameInitialization->getPlayers();
        gameEngine->gameState = gameEngine->gameInitialization->getGameState();
    }
    return gameEngine;
}

GameEngine::GameEngine(vector<Player *> players, Map *map, Deck *deck, GameState *gameState) {
    this->players = players;
    this->map = map;
    this->deck = deck;
    this->gameState = gameState;
}

GameEngine::~GameEngine() {
    delete gameInitialization;
}

// Startup phase logic
void GameEngine::startupPhase() {
    randomlySetOrder();
    assignTerritoriesToPlayers();
    assignArmiesToPlayers();
}

void GameEngine::randomlySetOrder() {

    cout << "Before shuffling, this is the order of players" << endl;
    for (auto &it : players)
        std::cout << ' ' << it->getPlayerName();

    cout << endl;

    // Randomize (shuffle) the order of the players
    shuffle(players.begin(), players.end(), std::mt19937(std::random_device()()));

    cout << "After shuffling, this is the order of players" << endl;
    for (auto &it : players)
        std::cout << ' ' << it->getPlayerName();

    cout << endl;
}

void GameEngine::assignTerritoriesToPlayers() {
    int territoriesAssigned = 0;
    vector<Territory *> territoriesAvailable = map->getTerritoryList();

    while (!territoriesAvailable.empty()) {
        // Pick a random territory
        int randomIndex = rand() % territoriesAvailable.size();
        Territory *territory = territoriesAvailable.at(randomIndex);
        // Remove it from available territories
        territoriesAvailable.erase(territoriesAvailable.begin() + randomIndex);
        // Assign using Round Robin Method
        territory->setOwner(players.at(territoriesAssigned % players.size()));
        cout << "Assigning territory " << territory->getTerritoryName() << " to "
             << players.at(territoriesAssigned % players.size())->getPlayerName() << endl;
        territoriesAssigned++;
    }
    cout << "All territories Assigned." << endl;
    cout << "==========================================" << endl;
}

void GameEngine::assignArmiesToPlayers() {
    int nmbArmy = getInitialArmyNumber();
    for (auto p : players) {
        p->setNumberOfArmies(nmbArmy);
        cout << p->getPlayerName() << " got assigned A = " << p->getNumberofArmies() << endl;
    }
}

int GameEngine::getInitialArmyNumber() {
    switch (players.size()) {
        case 2:
            return 40;
        case 3:
            return 35;
        case 4:
            return 30;
        case 5:
        default:
            return 25;
    }
}

// Main game loop logic
void GameEngine::mainGameLoop() {
    static int counter = 1;
    while (players.size() != 1) {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ROUND " << counter << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        // TODO: give cards to those who conquered stuff (IMPORTANT) --> save a vector<Player*> with state before and after execution and compare the territories size befrore and after. If size increased = conquered something. Then, give card. (VivekA + FERODU)
//        // Pick a card
//        Card *drawnCard = GameEngine::getInstance()->getDeck()->draw();
//        player->getHandOfCards()->addCard(drawnCard);
//
        removePlayersWithoutTerritoriesOwned();
        resetDiplomacy();
        counter++;
    }
}

// TODO: cout how much they receive additional in each round by observer (Tarek + Abhijit)
void GameEngine::reinforcementPhase() {
    for (Player *player: players) {
        int numberOfArmiesToGive = calculateNumberOfArmiesToGive(player);
        player->setNumberOfArmies(player->getNumberofArmies() + numberOfArmiesToGive);
        gameState->updateGameState(player, reinforcement, nullptr, nullptr);
    }
}

int GameEngine::calculateNumberOfArmiesToGive(Player *player) {
    const int calculatedArmyUnits = player->getTerritories().size() / 3;
    int numberOfArmiesToGive = std::max(calculatedArmyUnits, 3);
    return numberOfArmiesToGive + getBonus(player);
}

// TODO: cout when bonus is given by observers (Tarek + Abhijit)
int GameEngine::getBonus(Player *player) {
    set<Continent *> continentsWherePlayerOwnsTerritories;
    for (Territory *territory: player->getTerritories()) {
        int continentId = territory->getContinentId();
        continentsWherePlayerOwnsTerritories.insert(map->getContinentList().at(continentId - 1));
    }

    int controlValueBonus = 0;
    for (Continent *continent: continentsWherePlayerOwnsTerritories) {
        if (continent->getOwner() == player)
            controlValueBonus += continent->getBonus();
    }
    return controlValueBonus;
}

void GameEngine::issueOrdersPhase() {
    vector<Player *> playersWithNoMoreOrderstoIssue;
    while (playersWithNoMoreOrderstoIssue.size() != players.size()) {
        for (Player *player: players) {
            if (find(playersWithNoMoreOrderstoIssue.begin(), playersWithNoMoreOrderstoIssue.end(), player) ==
                playersWithNoMoreOrderstoIssue.end()) {
                if (!player->issueOrder()) {
                    playersWithNoMoreOrderstoIssue.push_back(player);
                    if (phaseObserverActive)
                        cout << player->getPlayerName() << " is done issuing orders!" << endl; // TODO: move this to the phase observer and remove if statement (Tarek)
                }
            }
        }
    }
}

void GameEngine::executeOrdersPhase() {
    // Prioritize the orders
    for (Player *player: players) {
        player->getOrders()->sortOrderListByPriority();
        cout << endl;
        cout << player->getPlayerName() << "\'s order list:\n"<<*player->getOrders() << endl;
    }

    // Execute all deploy orders
    set<Player *> playersWithNoMoreDeployOrderstoExecute;
    while (playersWithNoMoreDeployOrderstoExecute.size() != players.size()) {
        for (Player *player: players) {
            vector<Order *> &orderList = player->getOrders()->getOrderList();
            if (!orderList.empty()) {
                auto *deployOrder = dynamic_cast<DeployOrder *>(orderList[0]);
                if (deployOrder) {
                    deployOrder->execute();
                    gameState->updateGameState(player, orders_execution, deployOrder,nullptr);
                    player->getOrders()->remove(deployOrder);
                } else {
                    playersWithNoMoreDeployOrderstoExecute.insert(player);
                }
            } else {
                playersWithNoMoreDeployOrderstoExecute.insert(player);
            }
        }
    }

    // Execute the rest of the orders
    set<Player *> playersWithNoMoreOrdersToExecute;
    while (playersWithNoMoreOrdersToExecute.size() != players.size()) {
        for (Player *player: players) {
            vector<Order *> &orderList = player->getOrders()->getOrderList();
            if (!orderList.empty()) {
                orderList[0]->execute();
                gameState->updateGameState(player, orders_execution, orderList[0], nullptr);
                player->getOrders()->remove(orderList[0]);
                if (winnerExists())
                    return;
            } else {
                playersWithNoMoreOrdersToExecute.insert(player);
            }
        }
    }
}

bool GameEngine::winnerExists() {
    set<Player*> remainingPlayers;
    for(Player* player: players) {
        if(!player->getTerritories().empty())
            remainingPlayers.insert(player);
    }

    return remainingPlayers.size() == 1;
}

void GameEngine::removePlayersWithoutTerritoriesOwned() {
    for (Player *player: players) {
        if (player->getTerritories().empty()) {
            auto position = find(players.begin(), players.end(), player);
            if (position != players.end()) {
                players.erase(position);
                delete player;
                player = nullptr;
            }
        }
    }
}

// Getters
const vector<Player *> &GameEngine::getPlayers() const {
    return players;
}

Map *GameEngine::getMap() const {
    return map;
}

Deck *GameEngine::getDeck() const {
    return deck;
}

// Setters
void GameEngine::setPlayers(const vector<Player *> &players) {
    GameEngine::players = players;
}

void GameEngine::setMap(Map *map) {
    GameEngine::map = map;
}

void GameEngine::setDeck(Deck *deck) {
    GameEngine::deck = deck;
}

GameState *GameEngine::getGameState() const {
    return gameState;
}

void GameEngine::setGameState(GameState *gameState) {
    GameEngine::gameState = gameState;
}

bool GameEngine::isPhaseObserverActive() const {
    return phaseObserverActive;
}

void GameEngine::setPhaseObserverActive(bool phaseObserverActive) {
    GameEngine::phaseObserverActive = phaseObserverActive;
}

void GameEngine::resetDiplomacy() {
    for (auto player : players) {
        player->getPlayersNotToAttack().clear();
    }
}