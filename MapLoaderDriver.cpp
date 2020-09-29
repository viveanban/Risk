#include <iostream>
#include "MapLoader.h"
using namespace mapLoader;

int main() {

    // Asks user for a .map name

    // Check if file exists.
    // If not, ask user for another .map name
    // If exists, load map (check for invalid map, read map, construct and return Map object)

    std::cout << "Hello, World!" << std::endl;
    mapLoader::MapLoader loader = mapLoader::MapLoader();
    loader.readMap("../maps/Solar.map");

    return 0;
}
