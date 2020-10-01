#include <iostream>
#include "MapLoader/MapLoaderDriver.h"
#include "Map/MapDriver.h"

int main() {
    MapLoaderDriver mapLoaderDriver;
    Graph *map;
    map = mapLoaderDriver.run();

    MapDriver mapDriver;
    mapDriver.run(map);

    return 0;
}