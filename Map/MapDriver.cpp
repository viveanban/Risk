//#include "./Map.h"
//#include "./../MapLoader/MapLoader.h"
//
//using namespace std;
//
//int main() {
//    Map *mapValid = MapLoader::loadMap("Europe.map");
//    Map *mapUnconnected = MapLoader::loadMap("validation/EuropeUnconnected.map");
//    Map *mapDuplicateTerritory = MapLoader::loadMap("validation/DuplicateTerritory.map");
//    Map *mapIsolateContinent = MapLoader::loadMap("validation/IsolateContinent.map");
//
//    cout << "Validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
//    cout << "Validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
//    cout << "Validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
//    cout << "Validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;
//
//    cout << "--------Execution of Map Terminated--------" << endl;
//}
