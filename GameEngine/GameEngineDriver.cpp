#include "GameEngine.h"
#include "./../Map/Map.h"
#include "./../MapLoader/MapLoader.h"
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

const string MAP_DIRECTORY = "../maps/";

int main() {
    string chosenMap;
    int numberOfPlayers;
    ifstream inputFile;
    bool phaseObserver = false;
    bool statisticsObserver = false;
    Map *map;

    // 1)  select a map from a list of map files as stored in a directory.
    cout << "Welcome to Risk!" << endl;
    do {
        cout << "Please choose a game Map from the following list:" << endl;
        cout << "// TODO: list of maps" << endl;
        cin >> chosenMap;
        inputFile.open(MAP_DIRECTORY + chosenMap);
        while (!inputFile) {
            inputFile.close();
            cout << "Hey you made a mistake, " << chosenMap << " is not one of the choices." << endl;
            cout << "Please pick another map now: " << endl;
            cin >> chosenMap;
            inputFile.open(MAP_DIRECTORY + chosenMap);
        }
        map = MapLoader::loadMap(chosenMap);
    } while (!map->validate());
    inputFile.close();

    //  2) select the number of players in the game (2-5 players)
    cout << "Good choice, now please enter the number of players that will play this game (2 - 5 players)." << endl;
    cin >> numberOfPlayers;
    if (cin.fail()) {
        cin.clear();
        numberOfPlayers = 0;
        // discard 'bad' character(s)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while (numberOfPlayers < 2 or numberOfPlayers > 5) {
        cout << "Oopsies, that's not a number between 2 and 5. Try again ;)" << endl;
        cin >> numberOfPlayers;
        if (cin.fail()) {
            cin.clear();
            numberOfPlayers = 0;
            // discard 'bad' character(s)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

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

    cout << boolalpha << chosenMap << numberOfPlayers << phaseObserver << statisticsObserver << endl;
    //
//    cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//    cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//    cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//    cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//
//    cout << "--------Execution of Map Terminated--------" << endl;
}