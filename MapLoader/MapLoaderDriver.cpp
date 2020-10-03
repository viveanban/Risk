#include <fstream>
#include "MapLoader.h"


const string MAP_DIRECTORY = "../maps/";

int main() {
    // User chooses map
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

    // Load map
    MapLoader mapLoader;
    Graph *map;
    map = mapLoader.loadMap(userInput);

    return 0;
}

//bool fileExists(string mapFileName) {
//    ifstream infile(MAP_DIRECTORY + mapFileName);
//    return infile.good();
//}
