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

// GAME STARTUP PHASE
// ---------GAME ENGINE---------------
void GameEngine::initializeGame() {
    selectMap();
    selectPlayerNumber();
    setupObservers();
    setupPlayers();
    this->deck = new Deck(50);
    cout << *deck;
    gameState->setTotalTerritories(map->getTerritoryList().size());
}

void GameEngine::selectMap() {
    //TODO: Change the map directory according to the type of map
    const string MAP_DIRECTORY = "../maps/conquest_maps/";
    int chosenMap;
    ifstream inputFile;
    do {
        cout << "Please enter the number of the game Map you wish to play from the following list:" << endl;
        auto path = "../maps/conquest_maps/";
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
        MapLoader *mapLoader = new ConquestFileReaderAdapter();
        this->map = mapLoader->loadMap(availableMaps.at(chosenMap - 1));
        delete mapLoader;
        mapLoader = nullptr;
    } while (map == NULL or !map->validate());
    inputFile.close();
}

int GameEngine::openMapFile(const string &MAP_DIRECTORY, int chosenMap, ifstream &inputFile) const {
    cin >> chosenMap;
    if (cin.fail()) {
        cin.clear();
        chosenMap = -1;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (chosenMap > 0 and chosenMap <= availableMaps.size()) {
        inputFile.open(MAP_DIRECTORY + availableMaps.at(chosenMap - 1));
    }
    return chosenMap;
}

void GameEngine::setAvailableMaps(const char *path) {
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

int GameEngine::isRegularFile(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void GameEngine::selectPlayerNumber() {
    int numPlayerTmp = -1;
    int maxPlayerNumber = std::min( (int)map->getTerritoryList().size(), 5);
    cout << "The game supports up to "<< maxPlayerNumber <<" players with a minimum of 2."
            " Please input the desired number of players" << endl;
    numPlayerTmp = validateNumberPlayerInput(numPlayerTmp);
    while (numPlayerTmp < 2 or numPlayerTmp > maxPlayerNumber) {
        cout << "This does not look like a number between 2 to " << maxPlayerNumber <<
             ". The game supports up to "<< maxPlayerNumber <<" players with a minimum of 2." << endl <<
             "Please input the desired number of players" << endl;
        numPlayerTmp = validateNumberPlayerInput(numPlayerTmp);
    }
    this->numPlayer = numPlayerTmp;
}

int GameEngine::validateNumberPlayerInput(int numPlayerTmp) {
    cin >> numPlayerTmp;
    if (cin.fail()) {
        cin.clear();
        numPlayerTmp = 0;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return numPlayerTmp;
}

void GameEngine::setupObservers() {
    if (getTrueFalseInputFromUser("phase")) {
        gameState->attach(new PhaseObserver(gameState));
        phaseObserverActive = true;
    } else {
        phaseObserverActive = false;
    }

    if (getTrueFalseInputFromUser("statistics")) {
        gameState->attach(new StatisticsObserver(gameState));
        statisticsObserverActive = true;
    } else {
        statisticsObserverActive = false;
    }

}

bool GameEngine::getTrueFalseInputFromUser(string resultName) {
    bool result = false;
    do {
        cout << "Do you want to turn on the " << resultName << " observer [true/false]" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> boolalpha >> result;
    } while (cin.fail());
    return result;
}

void GameEngine::setupPlayers() {
    for (int i = 0; i < this->getNumPlayer(); i++) {
        this->players.push_back(new Player("Player " + to_string(i + 1)));
    }
}


bool GameEngine::isPhaseObserverActive() const {
    return phaseObserverActive;
}

bool GameEngine::isStatisticsObserverActive() const {
    return statisticsObserverActive;
}

int GameEngine::getNumPlayer() const {
    return numPlayer;
}

GameEngine::~GameEngine() {
    for (auto player: players) {
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

std::ostream &operator<<(ostream &stream, GameEngine &gameEngine) {
    return stream << "Information on GameEngine object:" << endl
                  << "map: " << gameEngine.getMap() << endl
                  << "Number of players: " << gameEngine.getPlayers().size() << endl
                  << "Statistic Observer on: " << boolalpha << gameEngine.isStatisticsObserverActive() << endl
                  << "Phase Observer on: " << boolalpha << gameEngine.isPhaseObserverActive() << endl
                  << "Deck: " << gameEngine.getDeck() << endl;
}

GameEngine *GameEngine::gameEngine = nullptr;

GameEngine::GameEngine() : availableMaps{}, statisticsObserverActive{}, phaseObserverActive{}, numPlayer{0}, players(),
                           map(nullptr), deck(nullptr), gameState(new GameState(0, nullptr, reinforcement)) {}

GameEngine *GameEngine::getInstance() {
    if (gameEngine == nullptr) {
        gameEngine = new GameEngine();
        gameEngine->initializeGame();
    }
    return gameEngine;
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
        p->setNumberOfArmiesInReinforcementPool(nmbArmy);
        cout << p->getPlayerName() << " got assigned A = " << p->getNumberofArmiesInReinforcementPool() << endl;
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
        removePlayersWithoutTerritoriesOwned();
        resetDiplomacy();
        counter++;
    }
}

void GameEngine::reinforcementPhase() {
    cout << "Entering Reinforcement Phase ..." << endl;
    for (Player *player: players) {
        int numberOfArmiesToGive = calculateNumberOfArmiesToGive(player);
        player->setNumberOfArmiesInReinforcementPool(
                player->getNumberofArmiesInReinforcementPool() + numberOfArmiesToGive);
        gameState->updateGameState(player, reinforcement, nullptr, nullptr);
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
    cout << "Entering Issuing Phase ..." << endl;
    vector<Player *> playersWithNoMoreOrderstoIssue;
    while (playersWithNoMoreOrderstoIssue.size() != players.size()) {
        for (Player *player: players) {
            if (find(playersWithNoMoreOrderstoIssue.begin(), playersWithNoMoreOrderstoIssue.end(), player) ==
                playersWithNoMoreOrderstoIssue.end()) {
                if (!player->issueOrder()) {
                    playersWithNoMoreOrderstoIssue.push_back(player);
                    gameState->updateGameState(player, issuing_orders, nullptr, nullptr);
                }
            }
        }
    }
}

void GameEngine::executeOrdersPhase() {
    cout << "Entering Execution Phase ..." << endl;
    // Save territory list sizes before execution
    vector<vector<int>> territoryIdsPerPlayerBeforeExecution;

    // Prioritize the orders
    for (Player *player: players) {
        player->getOrders()->sortOrderListByPriority();
        cout << endl;
        cout << player->getPlayerName() << "\'s order list:\n"<<*player->getOrders() << endl;

        vector<int> territoryIdsOfPlayer;
        for(Territory* territory: player->getTerritories())
        {
            territoryIdsOfPlayer.push_back(territory->getTerritoryId());
        }

        territoryIdsPerPlayerBeforeExecution.push_back(territoryIdsOfPlayer);
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
                player->getOrders()->remove(orderList[0]);
                if (winnerExists())
                    return;
            } else {
                playersWithNoMoreOrdersToExecute.insert(player);
            }
        }
    }

    // Check if any territory has been conquered by player
    for(int index = 0; index < players.size(); index++) {
        bool hasConqueredAtLeastOneTerritory = false;
        Player* player = players.at(index);
        vector<int> territoryIdsBeforeExecution = territoryIdsPerPlayerBeforeExecution.at(index);

        for(Territory* territory: player->getTerritories()) {
            auto position = find(territoryIdsBeforeExecution.begin(), territoryIdsBeforeExecution.end(), territory->getTerritoryId());
            if (position == territoryIdsBeforeExecution.end()) {
                hasConqueredAtLeastOneTerritory = true;
                break;
            }
        }

        if(hasConqueredAtLeastOneTerritory) {
            // Pick a card
            Card *drawnCard = GameEngine::getInstance()->getDeck()->draw();
            player->getHandOfCards()->addCard(drawnCard);
            cout << player->getPlayerName()
                 << " has conquered at least 1 territory in this round. They have drawn the "
                 << drawnCard->getTypeName() << " card!" << endl;
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

GameState *GameEngine::getGameState() const {
    return gameState;
}

void GameEngine::resetDiplomacy() {
    for (auto player : players) {
        player->getPlayersNotToAttack().clear();
    }
}