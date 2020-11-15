#include "GameEngine.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const string MAP_DIRECTORY = "../maps/";

int main() {
    srand(time(0));

    GameInitialization gameInitialization;
    gameInitialization.gameStart();

    GameEngine* gameEngine = GameEngine::getInstance();
    // TODO: abstract setters into a method
    gameEngine->setDeck(gameInitialization.getDeck());
    gameEngine->setMap(gameInitialization.getMap());
    gameEngine->setPlayers(gameInitialization.getPlayers());

    gameEngine->startupPhase();
    gameEngine->mainGameLoop();

    delete gameEngine;
}

// TODO: remove?
//#include "./../Map/Map.h"
//#include "./../MapLoader/MapLoader.h"
//#include <fstream>
//#include <limits>

//cout << boolalpha << gameInitialization.getMap() << endl;
//cout << gameInitialization.getNumPlayer() << endl;
//cout << gameInitialization.isPhaseObserver() << endl;
//cout << gameInitialization.isStatisticsObserver() << endl;
//
//cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//
//cout << "--------Execution of Map Terminated--------" << endl;