#include "./Map.h"
#include "./../MapLoader/MapLoader.h"

using namespace std;

int main() {
    MapLoader mapLoader;
    Graph *map;
    map = mapLoader.loadMap("Europe.map");

    //in order to break the continent-territory mapping
    //(*(*map).getContinentList().at(0)).setTerritories({});

    bool validateOutcome = (*map).validate();

    cout << "validate outcome: " << boolalpha << validateOutcome << endl;

//    Test for copy constructor
//    Territory t1 = *(map->getTerritoryList().at(0));
//    Territory t2 = Territory(t1);
//    t1.getAdjList().at(0) = {};
//    cout << t1.getAdjList().at(0) << " t2: " << t2.getAdjList().at(0);

    delete map;
}

