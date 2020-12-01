//#include <fstream>
//#include "MapLoader.h"
//
//void loadMaps(const vector<string>& mapsToValidate, MapLoader *mapLoader);
//
//int main() {
//    vector<string> conquestMapsToValidate = {"Asia.map", "Africa.map", "NY.map", "Oregon.map", "UnconnectedAfrica.map", "InvalidAfrica.map", "InvalidExtension.txt"};
//    vector<string> regularMapsToValidate = {"Europe.map", "USInvalid"};
//
//    MapLoader *conquestMapLoader = new ConquestFileReaderAdapter();
//    loadMaps(conquestMapsToValidate, conquestMapLoader);
//
//    MapLoader *mapLoader = new MapLoader();
//    loadMaps(regularMapsToValidate, mapLoader);
//
//    delete conquestMapLoader;
//    delete mapLoader;
//}
//
//void loadMaps(const vector<string>& mapsToValidate, MapLoader *mapLoader) {
//    for (const string& mapName: mapsToValidate) {
//        cout << "==========================================================" << endl;
//        cout << "Loading " << mapName << " from maps/conquest_maps/" << endl;
//        Map* map = mapLoader->loadMap(mapName);
//        if (map != nullptr) {
//            cout << *map << endl;
//            map->validate();
//        }
//        cout << "==========================================================" << endl;
//
//        delete map;
//        map = nullptr;
//    }
//}
