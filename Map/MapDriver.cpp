#include "./Map.h"
#include "./../MapLoader/MapLoader.h"
using namespace std;

int main(){
    MapLoader mapLoader;
    Graph *map;
    map = mapLoader.loadMap();

    //in order to break the continent-territory mapping
    //(*(*map).getContinentList().at(0)).setTerritories({});

    bool validateOutcome = (*map).validate();

    cout << "validate outcome: " << boolalpha << validateOutcome << endl;

}

