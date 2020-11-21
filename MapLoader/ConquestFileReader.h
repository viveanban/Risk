#ifndef RISK_CONQUESTFILEREADER_H
#define RISK_CONQUESTFILEREADER_H

#include <string>
#include <vector>
#include <iostream>
#include "../Map/Map.h"

class ConquestFileReader {
private:
    enum Section {
        other, continents, territories
    };

    static const string MAP_FILENAME_FORMAT_REGEX;

    static const string MAP_DIRECTORY;

    static const string CONTINENT_REGEX;

    static const string TERRITORY_REGEX;

    static const string BORDER_REGEX;

    static Section currentSection;

    static vector<Continent *> continentsList;

    static vector<Territory *> territoriesList;

    static void parseFile(fstream &mapFile);

    /**
     * This method keeps track and updates the current section in which the parsing is happening.
     *
     * @param string: the line being read.
     */
    static bool updateSection(string &line);

    /**
     * This method creates a Continent object by parsing a Continent entry from the file.
     *
     * @return Continent pointer.
     * @param string: the line being read, int: the continent id (reference).
     */
    static Continent *createContinents(const string &line, int &continentId);

    /**
     * This method creates a Territory object by parsing a Country entry from the file.
     *
     * @return Territory pointer.
     * @param string: the line being read.
     */
    static Territory *createTerritories(const string &line, int &territoryId);

    static void checkPattern(const string &line, const string &pattern);

    static void throwInvalidMapException();

public:
    ConquestFileReader() = default;

    ConquestFileReader(const ConquestFileReader &original);

    ConquestFileReader &operator=(const ConquestFileReader &original);

    ~ConquestFileReader();

    static Map *loadConquestMap(const string &conquestMapName);
};


#endif //RISK_CONQUESTFILEREADER_H
