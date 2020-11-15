//#include "GameEngine.h"
//#include "./../Map/Map.h"
//#include "./../MapLoader/MapLoader.h"
//#include <ctime>
//#include <iostream>
//#include <fstream>
//#include <limits>
//
//using namespace std;
//
//const string MAP_DIRECTORY = "../maps/";
//
//int main() {
//    srand(time(0));
//
//    GameInitialization gameInitialization;
//
//    gameInitialization.gameStart();
//
//    cout << boolalpha << gameInitialization.getMap() << endl;
//    cout << gameInitialization.getNumPlayer() << endl;
//    cout << gameInitialization.isPhaseObserver() << endl;
//    cout << gameInitialization.isStatisticsObserver() << endl;
//
//    cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//    cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//    cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//    cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//
//    cout << "--------Execution of Map Terminated--------" << endl;
//
//    GameEngine gameEngine(gameInitialization.getPlayers(), gameInitialization.getMap(), gameInitialization.getDeck());
//    gameEngine.startupPhase();
//    gameEngine.mainGameLoop();
////    // a player receives the correct number of armies in the reinforcement phase (showing different cases)
////
////    // a player will only issue deploy orders and no other kind of orders if they still have armies in their reinforcement pool
////
////    // the game engine will only execute non-deploy orders when all the deploy orders of all players have been executed
////
////    // a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists
////
////    // a player can play cards to issue orders
////
////    // a player that does not control any territory is removed from the game
////
////    // the game ends when a single player controls all the territories
//}