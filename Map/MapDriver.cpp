#include "./Map.h"
#include "./../MapLoader/MapLoader.h"

using namespace std;

int main() {
    Graph *mapValid = MapLoader::loadMap("Europe.map");
    Graph *mapUnconnected = MapLoader::loadMap("validation/EuropeUnconnected.map");
    Graph *mapDuplicateTerritory = MapLoader::loadMap("validation/DuplicateTerritory.map");
    Graph *mapIsolateContinent = MapLoader::loadMap("validation/IsolateContinent.map");

    cout << "validate outcome for known valid map:\n" << boolalpha << (*mapValid).validate() << endl;
    cout << "validate outcome for unconnected map:\n" << boolalpha << (*mapUnconnected).validate() << endl;
    cout << "validate outcome for map with duplicated territory:\n" << boolalpha << (*mapDuplicateTerritory).validate() << endl;
    cout << "validate outcome for map with unconnected continent:\n" << boolalpha << (*mapIsolateContinent).validate() << endl;

    // Test for Territory copy constructor
    Territory *t1 = mapValid->getTerritoryList().at(0);
    Territory *t2 = new Territory(*t1);
    cout <<  *t1 << endl;
    cout <<  *t2 << endl;

    delete mapValid;
    mapValid = nullptr;

    delete mapUnconnected;
    mapUnconnected = nullptr;

    delete mapDuplicateTerritory;
    mapDuplicateTerritory = nullptr;

    delete mapIsolateContinent;
    mapIsolateContinent = nullptr;

}
