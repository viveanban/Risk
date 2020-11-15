#include <fstream>
#include "GameEngine.h"
#include <set>
#include "../MapLoader/MapLoader.h"
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
    this->deck = new Deck(50);
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
//        TODO:  do you think we could pass the input file as a parameter directly instead of passing a string?
//         The inputFile is opened successfully when we get to the loadMap method but because we pass a string as parameter,
//         another file is opened in the scope of the loadMap
        this->map = MapLoader::loadMap(availableMaps.at(chosenMap - 1));
    } while (!map->validate());
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
    cout << "The game supports up to 5 players with a minimum of 2."
            " Please input the desired number of players" << endl;
    numPlayerTmp = validateNumberPlayerInput(numPlayerTmp);
    while (numPlayerTmp < 2 or numPlayerTmp > 5) {
        cout << "This does not look like a number between 2 to 5."
                "The game supports up to 5 players with a minimum of 2." << endl <<
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
    statisticsObserver = getTrueFalseInputFromUser("phase");
    statisticsObserver = getTrueFalseInputFromUser("statistics");
}

bool GameInitialization::getTrueFalseInputFromUser(string resultName) {
    bool result = false;
    do {
        cout << "do you want to turn on the " << resultName << " observer [true/false]" << endl;
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

// ---------GAME ENGINE---------------
GameEngine* GameEngine::gameEngine = nullptr;

GameEngine::GameEngine() : players(), map(nullptr), deck(nullptr) {}

GameEngine *GameEngine::getInstance()
{
    if(gameEngine == nullptr) {
        gameEngine = new GameEngine();
        GameInitialization gameInitialization;
        gameInitialization.initializeGame();
        gameEngine->deck = gameInitialization.getDeck();
        gameEngine->map = gameInitialization.getMap();
        gameEngine->players = gameInitialization.getPlayers();
    }
    return gameEngine;
}

GameEngine::~GameEngine() {
    delete map;
    delete deck;

    for (auto player: players) {
        delete player;
        player = nullptr;
    }
    players.clear();
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

    // Randomize (shuffle) the order of the players
    shuffle(players.begin(), players.end(), std::mt19937(std::random_device()()));

    cout << "After shuffling, this is the order of players" << endl;
    for (auto &it : players)
        std::cout << ' ' << it->getPlayerName();
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
        players.at(territoriesAssigned % players.size())->addTerritory(territory);
        cout << "assigning territory " << territory->getTerritoryName() << " to "
             << players.at(territoriesAssigned % players.size()) << endl;
        territoriesAssigned++;
    }
    cout << "All territories Assigned." << endl;
}

void GameEngine::assignArmiesToPlayers() {
    int nmbArmy = getInitialArmyNumber();
    for (auto p : players) {
        p->setNumberOfArmies(nmbArmy);
        cout << "Player " << p << "got assigned A = " << p->getNumberofArmies() << endl;
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
    };
}

// Main game loop logic
void GameEngine::mainGameLoop() {
    while (!winnerExists()) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        removePlayersWithoutTerritoriesOwned();
    }

}

void GameEngine::reinforcementPhase() {
    for (Player *player: players) {
        int numberOfArmiesToGive = calculateNumberOfArmiesToGive(player);
        player->setNumberOfArmies(numberOfArmiesToGive);
    }
}

int GameEngine::calculateNumberOfArmiesToGive(Player *player) {
    const int calculatedArmyUnits = player->getTerritories().size() / 3;
    int numberOfArmiesToGive = std::max(calculatedArmyUnits, 3);
    return numberOfArmiesToGive + getBonus(player);
}

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
                if (!player->issueOrder())
                    playersWithNoMoreOrderstoIssue.push_back(player);
            }
        }
    }
}

void GameEngine::executeOrdersPhase() {
    // Prioritize the orders
    for (Player *player: players) {
        player->getOrders()->sortOrderListByPriority();
    }

    // Execute all deploy orders
    vector<Player *> playersWithNoMoreDeployOrderstoExecute;
    while (playersWithNoMoreDeployOrderstoExecute.size() != players.size()) {
        for (Player *player: players) {
            vector<Order *> &orderList = player->getOrders()->getOrderList();
            if (!orderList.empty()) {
                auto *deployOrder = dynamic_cast<DeployOrder *>(orderList[0]);
                if (deployOrder) {
                    deployOrder->execute();
                    player->getOrders()->remove(deployOrder);
                } else {
                    playersWithNoMoreDeployOrderstoExecute.push_back(player);
                }
            }
        }
    }

    // Execute the rest of the orders
    vector<Player *> playersWithNoMoreOrdersToExecute;
    while (playersWithNoMoreOrdersToExecute.size() != players.size()) {
        for (Player *player: players) {
            vector<Order *> &orderList = player->getOrders()->getOrderList();
            if (!orderList.empty()) {
                orderList[0]->execute();
                player->getOrders()->remove(orderList[0]);
            } else {
                playersWithNoMoreOrdersToExecute.push_back(player);
            }
        }
    }
}

bool GameEngine::winnerExists() {
    return players.size() == 1;
}

void GameEngine::removePlayersWithoutTerritoriesOwned() {
    for (Player *player: players) {
        if(player->getTerritories().empty()) {
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
