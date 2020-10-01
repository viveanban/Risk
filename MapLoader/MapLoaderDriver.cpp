#include "MapLoader.h"
#include "MapLoaderDriver.h"

using std::cout;
using std::cin;

Graph * MapLoaderDriver::run() {
    // Load map
    MapLoader mapLoader;
    return mapLoader.loadMap();
}
