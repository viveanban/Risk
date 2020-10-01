#include "./Map.h"
#include "./MapDriver.h"
#include "./../MapLoader/MapLoader.h"
using namespace std;

void MapDriver::run(Graph *map){

    bool validateOutcome = (*map).validate();

    cout << "validate outcome: " << boolalpha << validateOutcome << endl;
}

