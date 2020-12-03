//#include <fstream>
//#include "MapLoader.h"
//
//void loadMaps(const vector<string>& mapsToValidate, MapLoader *mapLoader, string& mapPath);
//
//int main() {
//    vector<string> conquestMapsToValidate = {"Asia.map", "Africa.map", "NY.map", "Oregon.map", "UnconnectedAfrica.map", "InvalidAfrica.map", "InvalidExtension.txt"};
//    vector<string> regularMapsToValidate = {"Europe.map", "USInvalid"};
//
//    MapLoader *conquestMapLoader = new ConquestFileReaderAdapter();
//    string path = "maps/conquest_maps/";
//    loadMaps(conquestMapsToValidate, conquestMapLoader, path );
//
//    MapLoader *mapLoader = new MapLoader();
//    path =  "maps/domination_maps/";
//    loadMaps(regularMapsToValidate, mapLoader,path);
//
//    delete conquestMapLoader;
//    delete mapLoader;
//}
//
//void loadMaps(const vector<string>& mapsToValidate, MapLoader *mapLoader, string& mapPath) {
//    for (const string& mapName: mapsToValidate) {
//        cout << "==========================================================" << endl;
//        cout << "Loading " << mapName << " from " << mapPath << endl;
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
