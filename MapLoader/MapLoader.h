//
// Created by Viveka Anban on 24-09-2020.
//

#ifndef RISK_MAPLOADER_H
#define RISK_MAPLOADER_H

#include <string>
#include <vector>
#include <iostream>
#include "../Map/Map.h"

using namespace std;

/**
 * This class takes care of creating a Map object (the map) in the Risk game based on user input.
 */
class MapLoader {
private:
    MapLoader() = default;

    MapLoader(const MapLoader &original);

    MapLoader &operator=(const MapLoader &original);

    /**
     * This method creates the Map object and returns a pointer to it.
     *
     * @return void.
     * @param fstream: the file that will be parsed.
     */
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
    static Territory *createTerritories(const string &line);

    /**
     * This method populates the adjList vector of a Territory by parsing a Border entry from the file.
     *
     * @return void.
     * @param string: the line being read.
     static */
    static void constructAdjencyList(const string &line);

    /**
     * This method verifies that a parsed line follows a specific pattern depending under which section it resides.
     *
     * @return void.
     * @param string: the line being read, string: the regex pattern.
     */
    static void checkPattern(const string& line, const string& pattern);

    /**
     * This method exits the program with an error.
     *
     * @return void.
     * @param None.
     */
    static void exitWithError();

public:
    /**
     * This method creates the Map object and returns a pointer to it.
     *
     * @return Map pointer.
     */
    static Map *loadMap(const string& mapName);

    friend std::ostream &operator<<(std::ostream &stream, MapLoader &mapLoader);

};

#endif //RISK_MAPLOADER_H
