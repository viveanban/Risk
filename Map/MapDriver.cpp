//#include "./Map.h"
//#include "./../MapLoader/MapLoader.h"
//
//using namespace std;
//
//int main() {
//    Graph *mapValid = MapLoader::loadMap("Europe.map");
//    Graph *mapUnconnected = MapLoader::loadMap("validation/EuropeUnconnected.map");
//    Graph *mapDuplicateTerritory = MapLoader::loadMap("validation/DuplicateTerritory.map");
//    Graph *mapIsolateContinent = MapLoader::loadMap("validation/IsolateContinent.map");
//
//    cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//    cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//    cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//    cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//
//    cout << "--------Execution of Map Terminated--------" << endl;
//}
