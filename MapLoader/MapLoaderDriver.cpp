#include "MapLoader.h"
#include "MapLoaderDriver.h"

using std::cout;
using std::cin;

void MapLoaderDriver::run() {
    // Load map
    MapLoader mapLoader;
    mapLoader.loadMap();
}
