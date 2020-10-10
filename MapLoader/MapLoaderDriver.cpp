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
//    MapLoader mapLoader;
//    Graph *map;
//    map = mapLoader.loadMap(userInput);
//    cout << *map << endl;
//
//    Graph *mapValid = mapLoader.loadMap("Europe.map");
//    Graph *mapUnconnected = mapLoader.loadMap("EuropeUnconnected.map");
//    Graph *mapDuplicateTerritory = mapLoader.loadMap("DuplicateTerritory.map");
//    Graph *mapIsolateContinent = mapLoader.loadMap("IsolateContinent.map");
//    cout << "validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//    cout << "validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//    cout << "validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//    cout << "validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//    return 0;
//}
