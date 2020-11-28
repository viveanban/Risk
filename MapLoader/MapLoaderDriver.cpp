//TODO: Adapt old MapLoaderDriver to demonstrate MapLoader and ConquestFileReader

//TODO: Test ConquestFileReader with Asia.map (validation of connected continents fail; check if it's true)
//#include <fstream>
//#include "MapLoader.h"
//
//const string MAP_DIRECTORY = "../maps/";
//
//string getUserInput() {
//    string userInput = "";
//    ifstream infile;
//
//    cout << "Enter your desired map:  " << std::endl;
//    cin >> userInput;
//
//    infile.open(MAP_DIRECTORY + userInput);
//    while (!infile) {
//        cout << "This map does not exist. Please enter another map: \n";
//        cin >> userInput;
//        infile.open(MAP_DIRECTORY + userInput);
//    }
//
//    return userInput;
//}
//
//
//int main() {
//    cout << "Getting input from player.. \n";
//    string userInput = getUserInput();
//
//    cout << "Map is being loaded.. \n";
//    Graph *map;
//    map = MapLoader::loadMap(userInput);
//    cout << *map << endl;
//
//    delete map;
//    map = nullptr;
//}


