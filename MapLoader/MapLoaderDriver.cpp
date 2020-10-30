#include <fstream>
#include "MapLoader.h"

const string MAP_DIRECTORY = "../maps/";

string getUserInput() {
    string userInput = "";
    ifstream infile;

    cout << "Enter your desired map:  " << std::endl;
    cin >> userInput;

    infile.open(MAP_DIRECTORY + userInput);
    while (!infile) {
        cout << "This map does not exist. Please enter another map: \n";
        cin >> userInput;
        infile.open(MAP_DIRECTORY + userInput);
    }

    return userInput;
}


int main() {
    cout << "Getting input from player.. \n";
    string userInput = getUserInput();

    cout << "Map is being loaded.. \n";
    Map *map;
    map = MapLoader::loadMap(userInput);
    cout << *map << endl;

    cout << "--------Execution of MapLoader Terminated--------" << endl;
}
