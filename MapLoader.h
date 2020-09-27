//
// Created by Viveka Anban on 24-09-2020.
//

#ifndef RISK_MAPLOADER_H
#define RISK_MAPLOADER_H

#include <string>
#include <vector>

using std::string;
using std::ifstream;
using std::vector;

namespace mapLoader {
    class MapLoader {
    public:
        MapLoader();

        void readMap(string file_path);

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
        bool parseFile(ifstream &mapFile);

        void skipIrrelevantLines(ifstream &mapFile, string line);

        bool isSectionRelevant(string line);

        bool updateCategory(string line);

        Continent createContinentFromLine(const string& line);
        Country createCountryFromLine(const string& line);
        Borders createBordersFromLine(const string& line);

        long getInt(const char *token, long& convertedToken);
    };
}
#endif //RISK_MAPLOADER_H
