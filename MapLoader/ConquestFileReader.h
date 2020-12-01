#ifndef RISK_CONQUESTFILEREADER_H
#define RISK_CONQUESTFILEREADER_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "../Map/Map.h"

//TODO: As per the TA's recommendation, join this file with the maploader.cpp/h
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

    static map<string, Continent*> nameToContinentMap;

    static map<string, Territory*> nameToTerritoryMap;

    static map<Territory*, set<string>> adjacentTerritoryNamesMap;

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
    * @param line being read
    * @param continentId
    * @return pointer of the created Continent
    */
    static Continent *createContinents(const string &line, int &continentId);

    /**
     * This method creates a Territory object by parsing a Territory entry from the file.
     *
     * @param line being read
     * @param territoryId
     * @return pointer of the created Territory
     */
    static Territory *createTerritories(const string &line, int &territoryId);

    /**
     * This method populates the adjList of each territory that has been created
     */
    static void constructAdjencyList();


public:
    ConquestFileReader() = default;

    ConquestFileReader(const ConquestFileReader &original);

    ConquestFileReader &operator=(const ConquestFileReader &original);

    friend ostream &operator<<(ostream &os, const ConquestFileReader &reader);

    ~ConquestFileReader();

    static Map *loadConquestMap(const string &conquestMapName);
};


#endif //RISK_CONQUESTFILEREADER_H
