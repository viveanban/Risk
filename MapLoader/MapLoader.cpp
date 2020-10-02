#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string.h>
#include <regex>

using namespace std;

/**
 * MapLoader Class implementation
 */

const string MAP_DIRECTORY = "../maps/";
const string CONTINENT_REGEX = "([A-Z]|[a-z]|_|-)+\\s+(\\d+|\\d+\\s.*)";
const string COUNTRY_REGEX = "\\d+\\s+([A-Z]|[a-z]|_|-)+\\s+(\\d+|\\d+\\s.*)";
const string BORDER_REGEX = "(\\d+\\s+)+\\d+";

enum Section {
    other, continents, countries, borders
};

Section currentSection;
vector<Continent *> continentsList; // Composed of pointers b/c we want to point to 1 single continent object instead of creating new ones
vector<Territory *> territoriesList; // Vectors are dynamic array so they are in the heap (stack has static size)

Graph *MapLoader::loadMap() {
    // 1. User chooses map
    string userInput = "";
    cout << "Enter your desired map:  " << std::endl;
    cin >> userInput;
    while (!fileExists(userInput)) {
        cout << "This map does not exist. Please enter another map: \n";
        cin >> userInput;
    }

    // 2. Read map
    string mapName = userInput;
    fstream mapFile;
    mapFile.open(MAP_DIRECTORY + mapName, ios::in | ios::binary);

    if (mapFile.is_open()) {
        parseFile(mapFile);
        mapFile.close();
    }

    // 3. Construct Graph object
    Graph *graph = new Graph(territoriesList, continentsList);

    return graph;
}

void MapLoader::parseFile(fstream &mapFile) {
    string line;
    int continentId = 1;

    while (getline(mapFile, line)) {
        line.erase(line.find_last_not_of(" \n\r\t") + 1);
        if (line.empty() || line.at(0) == ';')
            continue;

        bool isUpdated = updateSection(line);

        if (!isUpdated) {
            if (currentSection == continents) {
                checkPattern(line, CONTINENT_REGEX);
                continentsList.push_back(createContinents(line, continentId));
            } else if (currentSection == countries) {
                checkPattern(line, COUNTRY_REGEX);
                territoriesList.push_back(createTerritories(line));
            } else if (currentSection == borders) {
                checkPattern(line, BORDER_REGEX);
                createAdjencyList(line);
            }
        }
    }
}

bool MapLoader::updateSection(string &line) {
    if (line.at(0) == '[') {
        if (line.compare("[continents]") == 0) {
            currentSection = continents;
        } else if (line.compare("[countries]") == 0) {
            currentSection = countries;
        } else if (line.compare("[borders]") == 0) {
            currentSection = borders;
        } else {
            currentSection = other;
        }
        return true;
    }

    return false;
}

Continent *MapLoader::createContinents(const string &line, int &continentId) {
    const char *token = strtok((char *) line.c_str(), " ");
    int counter = 0;
    Continent *continent = new Continent(); // must create with new operator or else will be deleted at end of the method
    while (token != NULL) {
        if (counter == 0) {
            continent->setContinentName(token);
        } else if (counter == 1) {
            continent->setBonus(atoi(token));
        }
        token = strtok(NULL, " ");
        counter++;
    }

    continent->setContinentId(continentId++);

    return continent;
}

Territory *MapLoader::createTerritories(const string &line) {
    if(continentsList.empty()) exitWithError();

    const char *token = strtok((char *) line.c_str(), " ");
    int counter = 0;
    Territory *territory = new Territory();
    while (token != NULL) {
        if (counter == 0) {
            territory->setTerritoryId(atoi(token));
        } else if (counter == 1) {
            territory->setTerritoryName(token);
        } else if (counter == 2) {
            territory->setContinentId(atoi(token));
        }
        token = strtok(NULL, " ");
        counter++;
    }

    // TODO: break into two
    // return the continent with the specified id
    continentsList.at((territory->getContinentId()) - 1)->getTerritories().push_back(
            territory); // getTerritories returns an address to the real vector list b/c or else if would return a copy of the vector list which is not what we want

    return territory;
}

// TODO: change method name
void MapLoader::createAdjencyList(const string &line) {
    if (territoriesList.empty()) exitWithError();

    const char *token = strtok((char *) line.c_str(), " ");
    int counter = 0;
    int territoryId;
    while (token != NULL) {
        if (counter == 0) {
            territoryId = atoi(token);
        } else if (counter >= 1) {
            int borderId = atoi(token);
            //TODO: Find the territories based on id rather than index
            territoriesList.at(territoryId - 1)->getAdjList().push_back(
                    territoriesList.at(borderId - 1)); // TODO: break into two
        }
        token = strtok(NULL, " ");
        counter++;
    }
}

void MapLoader::checkPattern(string line, string pattern) {
    if (!regex_match (line,regex(pattern))) {
        exitWithError();
    }
}

bool MapLoader::fileExists(string mapFileName) {
    ifstream infile(MAP_DIRECTORY + mapFileName);
    return infile.good();
}

void MapLoader::exitWithError() {
    cout << "This map is invalid!" << endl;
    exit(EXIT_FAILURE);
}