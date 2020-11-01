#include <fstream>
#include "GameEngine.h"
#include "../MapLoader/MapLoader.h"
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::__fs::filesystem;


GameEngine::GameEngine(Map *map, Deck *deck, vector<Player *> &players) : map(map), deck(deck),
                                                                          players(players) {}


void GameSetup::selectMap() {
    const string MAP_DIRECTORY = "../maps/";
    int chosenMap;
    ifstream inputFile;
    do {
        cout << "Please choose a game Map from the following list:" << endl;
        std::string path = "../maps/";
        setAvailableMaps(path);
        for(int i=1; i <= availableMaps.size();i++){
            cout << i << " - " << availableMaps.at(i-1)<< endl;
        }
        cin >> chosenMap;
        inputFile.open(MAP_DIRECTORY + availableMaps.at(chosenMap-1));
        while (!inputFile) {
            inputFile.close();
            cout << "Hey you made a mistake, " << chosenMap << " is not one of the choices." << endl;
            cout << "Please pick another map now: " << endl;
            cin >> chosenMap;
            inputFile.open(MAP_DIRECTORY + availableMaps.at(chosenMap-1));
        }
        this->map = MapLoader::loadMap(availableMaps.at(chosenMap-1));
    } while (!map->validate());
    inputFile.close();
}

void GameSetup::setAvailableMaps(const string &path) {
    for (const auto & entry : fs::directory_iterator(path))
        if(!entry.is_directory())
        this->availableMaps.push_back(entry.path().filename());
}

void GameSetup::selectPlayerNumber() {

}

void GameSetup::gameStart() {
    selectMap();
    selectPlayerNumber();
}
