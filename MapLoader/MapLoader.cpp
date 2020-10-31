#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <regex>

using namespace std;

/**
 * MapLoader Class implementation
 */
const string MAP_FILENAME_FORMAT_REGEX = "[^.]+\\.+map";
const string MAP_DIRECTORY = "../maps/";
const string CONTINENT_REGEX = "([A-Z]|[a-z]|_|-)+\\s+(\\d+|\\d+\\s.*)";
const string COUNTRY_REGEX = "\\d+\\s+([A-Z]|[a-z]|_|-)+\\s+(\\d+|\\d+\\s.*)";
const string BORDER_REGEX = "(\\d+\\s+)+\\d+";

enum Section {
    other, continents, countries, borders
};

Section currentSection;
vector<Continent *> continentsList;
vector<Territory *> territoriesList;

MapLoader::MapLoader(const MapLoader &original) : MapLoader() {}

MapLoader &MapLoader::operator=(const MapLoader &original) { return *this; }

std::ostream &operator<<(ostream &stream, MapLoader &mapLoader) {
    return stream << "MapLoader: [continentList size =" << continentsList.size()
                  << ", territoriesList size = " << continentsList.size() << "]" << endl;
}

Map *MapLoader::loadMap(const string &mapName) {
    // Have a clear setup when loading a new map
    continentsList.clear();
    territoriesList.clear();

    // Read map
    fstream mapFile;
    checkPattern(mapName, MAP_FILENAME_FORMAT_REGEX);

    mapFile.open(MAP_DIRECTORY + mapName, ios::in);

    if (mapFile.is_open()) {
        parseFile(mapFile);
        mapFile.close();
    }
    else
        exitWithError();

    // Construct Map object
    auto *graph = new Map(territoriesList, continentsList);

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
                constructAdjencyList(line);
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
    auto *continent = new Continent(); // must create with new operator or else will be deleted at end of the method
    while (token != nullptr) {
        if (counter == 0) {
            continent->setContinentName(token);
        } else if (counter == 1) {
            continent->setBonus(atoi(token));
        }
        token = strtok(nullptr, " ");
        counter++;
    }

    continent->setContinentId(continentId++);

    return continent;
}

Territory *MapLoader::createTerritories(const string &line) {
    if (continentsList.empty()) exitWithError();

    const char *token = strtok((char *) line.c_str(), " ");
    int counter = 0;
    auto *territory = new Territory();
    while (token != nullptr) {
        if (counter == 0) {
            territory->setTerritoryId(atoi(token));
        } else if (counter == 1) {
            territory->setTerritoryName(token);
        } else if (counter == 2) {
            territory->setContinentId(atoi(token));
        }
        token = strtok(nullptr, " ");
        counter++;
    }

    continentsList.at(territory->getContinentId() - 1)->getTerritories().push_back(territory);

    return territory;
}

void MapLoader::constructAdjencyList(const string &line) {
    if (territoriesList.empty()) exitWithError();

    const char *token = strtok((char *) line.c_str(), " ");
    int counter = 0;
    int territoryId;
    while (token != nullptr) {
        if (counter == 0) {
            territoryId = atoi(token);
        } else if (counter >= 1) {
            int borderId = atoi(token);
            for (Territory *territory : territoriesList) {
                if (territory->getTerritoryId() == territoryId) {
                    for (Territory *border : territoriesList) {
                        if (border->getTerritoryId() == borderId) {
                            territory->getAdjList().push_back(border);
                            break;
                        }
                    }
                }
            }
        }
        token = strtok(nullptr, " ");
        counter++;
    }
}

void MapLoader::checkPattern(const string &line, const string &pattern) {
    if (!regex_match(line, regex(pattern))) {
        exitWithError();
    }
}

void MapLoader::exitWithError() {
    cout << "This map is invalid!" << endl;
    exit(EXIT_FAILURE);
}