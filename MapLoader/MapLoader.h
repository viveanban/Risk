//
// Created by Viveka Anban on 24-09-2020.
//

#ifndef RISK_MAPLOADER_H
#define RISK_MAPLOADER_H

#include <string>
#include <vector>
#include <iostream>
#include "../Map/Map.h"

using std::string;
using std::fstream;
using std::vector;
using std::cout;
using std::endl;

class MapLoader {
public:
    Graph * loadMap();

private:
    void parseFile(fstream &mapFile);
    bool updateCategory(string &line);

    Continent * createContinents(const string& line, int *continentId);
    Territory * createTerritories(const string& line);
    void createAdjencyList(const string& line);

    bool fileExists(string mapFileName);
};

#endif //RISK_MAPLOADER_H
