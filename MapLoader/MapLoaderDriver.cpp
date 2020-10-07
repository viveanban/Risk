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

//int main() {
//    // Load map
//    MapLoader mapLoader;
//    Graph *map;
//    map = mapLoader.loadMap(getUserInput());
//
//    return 0;
//}
