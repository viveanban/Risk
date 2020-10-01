#include "./Map.h"
#include "./MapDriver.h"
#include "./../MapLoader/MapLoader.h"
using namespace std;

void MapDriver::run(Graph *map){

    (*(*map).getContinentList().at(0)).setTerritories({});

    bool validateOutcome = (*map).validate();

    cout << "validate outcome: " << boolalpha << validateOutcome << endl;
}

