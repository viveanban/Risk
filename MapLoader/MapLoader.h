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
 * This class takes care of creating a Graph object (the map) in the Risk game based on user input.
 */
class MapLoader {
public:
    /**
     * This method creates the Graph object and returns a pointer to it.
     *
     * @return Graph pointer.
     */
    Graph *loadMap();

private:
    /**
     * This method creates the Graph object and returns a pointer to it.
     *
     * @return void.
     * @param fstream: the file that will be parsed.
     */
    void parseFile(fstream &mapFile);

    /**
     * This method keeps track and updates the current section in which the parsing is happening.
     *
     * @param string: the line being read.
     */
    bool updateSection(string &line);

    /**
     * This method creates a Continent object by parsing a Continent entry from the file.
     *
     * @return Continent pointer.
     * @param string: the line being read, int: the continent id (reference).
     */
    Continent *createContinents(const string &line, int &continentId);

    /**
     * This method creates a Territory object by parsing a Country entry from the file.
     *
     * @return Territory pointer.
     * @param string: the line being read.
     */
    Territory *createTerritories(const string &line);

    /**
     * This method populates the adjList vector of a Territory by parsing a Border entry from the file.
     *
     * @return void.
     * @param string: the line being read.
     */
    void createAdjencyList(const string &line);

    /**
     * This method verifies that a parsed line follows a specific pattern depending under which section it resides.
     *
     * @return void.
     * @param string: the line being read, string: the regex pattern.
     */
    void checkPattern(string line, string pattern);

    /**
     * This method verifies that the file exists in the program.
     *
     * @return boolean.
     * @param string: the name of the file.
     */
    bool fileExists(string mapFileName);

    /**
     * This method exits the program with an error.
     *
     * @return void.
     * @param None.
     */
    void exitWithError();

};

#endif //RISK_MAPLOADER_H
