#include "./Map.h"
#include "./../MapLoader/MapLoader.h"

using namespace std;

//int main() {
//    MapLoader mapLoader;
//    Graph *mapValid = mapLoader.loadMap("Europe.map");
////    Graph *mapUnconnected = mapLoader.loadMap("validation/EuropeUnconnected.map");
////    Graph *mapDuplicateTerritory = mapLoader.loadMap("validation/DuplicateTerritory.map");
////    Graph *mapIsolateContinent = mapLoader.loadMap("validation/IsolateContinent.map");
//
//    cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl << endl;
////    cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl << endl;
////    cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl << endl;
////    cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl << endl;
//
////    Test for copy constructor
////    Territory t1 = *(mapValid->getTerritoryList().at(0));
////    Territory t2 = Territory(t1);
////    t1.getAdjList().at(0) = {};
////    cout << t1.getAdjList().at(0) << " t2: " << t2.getAdjList().at(0);
//
////    delete mapValid;
////    delete mapUnconnected;
////    delete mapDuplicateTerritory;
////    delete mapIsolateContinent;
////
////    mapValid = nullptr;
////    mapUnconnected = nullptr;
////    mapDuplicateTerritory = nullptr;
////    mapIsolateContinent = nullptr;
//}
