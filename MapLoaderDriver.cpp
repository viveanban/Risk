#include <iostream>
#include "MapLoader.h"
using namespace mapLoader;

int main() {
    std::cout << "Hello, World!" << std::endl;
    mapLoader::MapLoader loader = mapLoader::MapLoader();
    loader.readMap("../test.map");


    return 0;
}
