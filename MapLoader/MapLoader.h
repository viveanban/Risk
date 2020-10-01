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
    void loadMap(); // TODO: return Map object

private:
    bool fileExists(string mapFileName);

    void parseFile(fstream &mapFile);

    void skipIrrelevantLines(fstream &mapFile, string &line);

    bool isSectionRelevant(string &line);

    bool updateCategory(string &line);

    Continent * createContinents(const string& line, int *continentId);
    Territory * createTerritories(const string& line);
    void createAdjencyList(const string& line);
};

#endif //RISK_MAPLOADER_H
