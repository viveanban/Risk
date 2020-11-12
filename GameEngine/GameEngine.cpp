#include <fstream>
#include "GameEngine.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/Player.h"
#include <random>
#include <string>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

//GAME INITIALIZATION PHASE

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

int GameInitialization::validateNumberPlayerInput(int numPlayerTmp) const {
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

    //  3) turn on/off any of the observers as described in Part 5
    do {
        cin.clear();
        phaseObserver = false;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ok almost done, do you want to turn on the phase observer [true/false]?" << endl;
        cin >> boolalpha >> phaseObserver;
    } while (cin.fail());

    do {
        cin.clear();
        statisticsObserver = false;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ok last step, do you want to turn on the statistics observer [true/false]?" << endl;
        cin >> boolalpha >> statisticsObserver;
    } while (cin.fail());
}

void GameInitialization::gameStart() {
    selectMap();
    selectPlayerNumber();
    setupObservers();

    setupPlayers();
    setupOrders();
    assignCards();

}

Map *GameInitialization::getMap() const {
    return map;
}

void GameInitialization::setMap(Map *map) {
    GameInitialization::map = map;
}

Deck *GameInitialization::getDeck() const {
    return deck;
}

void GameInitialization::setDeck(Deck *deck) {
    GameInitialization::deck = deck;
}

const vector<Player *> &GameInitialization::getPlayers() const {
    return players;
}

void GameInitialization::setPlayers(const vector<Player *> &players) {
    GameInitialization::players = players;
}

const vector<string> &GameInitialization::getAvailableMaps() const {
    return availableMaps;
}

void GameInitialization::setAvailableMaps1(const vector<string> &availableMaps) {
    GameInitialization::availableMaps = availableMaps;
}

bool GameInitialization::isPhaseObserver() const {
    return phaseObserver;
}

void GameInitialization::setPhaseObserver(bool phaseObserver) {
    GameInitialization::phaseObserver = phaseObserver;
}

bool GameInitialization::isStatisticsObserver() const {
    return statisticsObserver;
}

void GameInitialization::setStatisticsObserver(bool statisticsObserver) {
    GameInitialization::statisticsObserver = statisticsObserver;
}

int GameInitialization::getNumPlayer() const {
    return numPlayer;
}

void GameInitialization::setNumPlayer(int numPlayer) {
    GameInitialization::numPlayer = numPlayer;
}

void GameInitialization::setupPlayers() {

    for (int i = 0; i < this->getNumPlayer(); i++) {
        this->players.push_back(new Player());
    }
}

void GameInitialization::setupOrders() {
    for (auto player : players) {
        player->setOrders(new OrdersList());
    }
}
// TODO: check how many cards the deck should start with, is 50 the right number?
void GameInitialization::assignCards() {
    this->deck = new Deck(50);
    for (auto player : players) {
        player->setHandOfCards(new Hand());
    }
}

//GAME STARTUP PHASE

GameSetup::GameSetup(vector<Player *> players, Map *map) {
    this->listOfPlayers = players;
    this->map = map;
}

void GameSetup::startupPhase() {
    randomlySetOrder();
    assignCountriesToPlayers();
    assignArmiesToPlayers();
}

// TODO: should we add a static playerNumber variable to the Player class to set Player.playerName as player{1,2,3..} and increment everytime we make a player.
void GameSetup::randomlySetOrder() {

    cout << "Before shuffling, this is the order of players" << endl;
    for (auto &it : listOfPlayers)
        std::cout << ' ' << it;

//  Randomize (shuffle) the order of the players.
    shuffle(listOfPlayers.begin(), listOfPlayers.end(), std::mt19937(std::random_device()()));

    cout << "After shuffling, this is the order of players" << endl;
    for (auto &it : listOfPlayers)
        std::cout << ' ' << it;
}

void GameSetup::assignCountriesToPlayers() {
    int territoriesAssigned = 0;
    vector<Territory *> territoriesAvailable = map->getTerritoryList(); //TODO: check if modifying this list modifies the map's list

    while (!territoriesAvailable.empty()) {
        // pick a random territory
        int randomIndex = rand() % territoriesAvailable.size();
        Territory *territory = territoriesAvailable.at(randomIndex);
        // remove it from available territories
        territoriesAvailable.erase(territoriesAvailable.begin() + randomIndex);
        // assign using Round Robin Method
        listOfPlayers.at(territoriesAssigned % listOfPlayers.size())->addTerritory(territory);
        cout << "assigning territory " << territory->getTerritoryName() << " to "
             << listOfPlayers.at(territoriesAssigned % listOfPlayers.size()) << endl;
        territoriesAssigned++;
    }
    if (territoriesAssigned == map->getTerritoryList().size()) {
        cout << "All territories Assigned." << endl;
    } else {
        cout << "Error during territory assignment, not all territories assigned." << endl;
    }
}

void GameSetup::assignArmiesToPlayers() {
    int nmbArmy = getInitialArmyNumber();
    for (auto p : this->listOfPlayers) {
        p->setNumberOfArmies(nmbArmy);
        cout << "Player " << p << "got assigned A = " << p->getNumberOfArmies() << endl;
    }
}

int GameSetup::getInitialArmyNumber() {
    switch (this->listOfPlayers.size()) {
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


