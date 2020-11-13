#include "GameEngine.h"
#include "./../Map/Map.h"
#include "./../MapLoader/MapLoader.h"

#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

const string MAP_DIRECTORY = "../maps/";

int main() {

    GameInitialization gameInitialization;

    gameInitialization.gameStart();

    cout << boolalpha << gameInitialization.getMap() << endl;
    cout << gameInitialization.getNumPlayer() << endl;
    cout << gameInitialization.isPhaseObserver() << endl;
    cout << gameInitialization.isStatisticsObserver() << endl;

    GameSetup gameSetup(gameInitialization.getPlayers(), gameInitialization.getMap(), gameInitialization.getDeck());
    gameSetup.startupPhase();
//    cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//    cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//    cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//    cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//
//    cout << "--------Execution of Map Terminated--------" << endl;
}