#include "MapLoader.h"

int main() {
    // Load map
    MapLoader mapLoader;
    Graph *map;
    map = mapLoader.loadMap();

    return 0;
}
