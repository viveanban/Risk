//
// Created by Viveka Anban on 24-09-2020.
//

#ifndef RISK_MAPLOADER_H
#define RISK_MAPLOADER_H

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::fstream;
using std::vector;
using std::cout;
using std::endl;

class MapLoader {
public:
    void loadMap(); // TODO: return Map object

    // TODO: remove when merged with Map stuff
    struct Continent {
        string name;
        int bonus;
    };

    struct Country {
        int id;
        string name;
        int continentId;
    };

    struct Borders {
        int countryId;
        vector<int> adjacentCountries;
    };

private:
    bool fileExists(string mapFileName);

    bool parseFile(fstream &mapFile);

    void skipIrrelevantLines(fstream &mapFile, string &line);

    bool isSectionRelevant(string &line);

    bool updateCategory(string &line);

    Continent createContinentFromLine(const string& line); // TODO: return Continent
    Country createCountryFromLine(const string& line); // TODO: return Territory
    Borders createBordersFromLine(const string& line); // TODO: return Border

    long getInt(const char *token, long& convertedToken);
};

#endif //RISK_MAPLOADER_H
