////TODO: Test ConquestFileReader with Asia.map (validation of connected continents fail; check if it's true)
//#include <fstream>
//#include "MapLoader.h"
//
//int main() {
//
//    string conquestMapsToValidate[] = {"Asia.map", "Africa.map", "NY.map", "Oregon.map", "UnconnectedAfrica.map", "InvalidAfrica.map"};
//    string regularMapsToValidate[] = {"Europe.map", "USInvalid"};
//    Map *map;
//    ConquestFileReader conquestFileReader;
//    MapLoader *conquestMapLoader = new ConquestFileReaderAdapter(conquestFileReader);
//    MapLoader *mapLoader = new MapLoader();
//
//    for (const string& mapName: conquestMapsToValidate) {
//        cout << "==========================================================" << endl;
//        cout << "Loading " << mapName << " from maps/conquest_maps/" << endl;
//        map = conquestMapLoader->loadMap(mapName);
//        if (map != nullptr) {
//            cout << *map << endl;
//            map->validate();
//        }
//        cout << "==========================================================" << endl;
//
//    }
//    for (const string& mapName: regularMapsToValidate) {
//        cout << "==========================================================" << endl;
//        cout << "Loading " << mapName << " from maps/" << endl;
//        map = mapLoader->loadMap(mapName);
//        if (map != nullptr) {
//            cout << *map << endl;
//            map->validate();
//        }
//        cout << "==========================================================" << endl;
//
//    }
//
//    delete map;
//    delete conquestMapLoader;
//    delete mapLoader;
//}
//
//
