#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <regex>
#include <set>

using namespace std;
// ================================ MapLoader  =========================================================================
const string MapLoader::MAP_FILENAME_FORMAT_REGEX = "[^.]+\\.+map";
const string MapLoader::MAP_DIRECTORY = "../maps/domination_maps/";
const string MapLoader::CONTINENT_REGEX = "([A-Z]|[a-z]|_|-|[0-9])+\\s+(\\d+|\\d+\\s.*)";
const string MapLoader::COUNTRY_REGEX = "\\d+\\s+([A-Z]|[a-z]|_|-|[0-9])+\\s+(\\d+|\\d+\\s.*)";
const string MapLoader::BORDER_REGEX = "(\\d+\\s+)+\\d+";

MapLoader::Section MapLoader::currentSection{};
vector<Continent *> MapLoader::continentsList{};
vector<Territory *> MapLoader::territoriesList{};

MapLoader::MapLoader(const MapLoader &original) : MapLoader() {}

MapLoader &MapLoader::operator=(const MapLoader &original) = default;

std::ostream &operator<<(ostream &stream, MapLoader &mapLoader) {
    return stream << "MapLoader: [continentList size =" << MapLoader::continentsList.size()
                  << ", territoriesList size = " << MapLoader::territoriesList.size() << "]" << endl;
}

Map *MapLoader::loadMap(const string &mapName) {
    // Have a clear setup when loading a new map
    continentsList.clear();
    territoriesList.clear();
    try {
        // Read map
        fstream mapFile;
        checkPattern(mapName, MAP_FILENAME_FORMAT_REGEX);

        mapFile.open(MAP_DIRECTORY + mapName, ios::in);

        if (mapFile.is_open()) {
            parseFile(mapFile);
            mapFile.close();
        } else
            throwInvalidMapException();

        // Construct Map object
        auto *graph = new Map(territoriesList, continentsList);

        return graph;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
        return nullptr;
    }
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
                continentId++;
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
        if (line == "[continents]") {
            currentSection = continents;
        } else if (line == "[countries]") {
            currentSection = countries;
        } else if (line == "[borders]") {
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

    continent->setContinentId(continentId);

    return continent;
}

Territory *MapLoader::createTerritories(const string &line) {
    if (continentsList.empty()) throwInvalidMapException();

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
    if (territoriesList.empty()) throwInvalidMapException();

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
        throwInvalidMapException();
    }
}

void MapLoader::throwInvalidMapException() {
    throw invalid_argument("Error loading map, this map format is invalid!");
}

// ================================ ConquestFileReader  ================================================================
const string ConquestFileReader::MAP_FILENAME_FORMAT_REGEX = "[^.]+\\.+map";
const string ConquestFileReader::MAP_DIRECTORY = "../maps/conquest_maps/";
const string ConquestFileReader::CONTINENT_REGEX = "([^\\r\\n])+=[0-9]+";
const string ConquestFileReader::TERRITORY_REGEX = "([^\\r\\n])+,[0-9]+,[0-9]+,([^\\r\\n])+(,([^\\r\\n])+)*";

ConquestFileReader::Section ConquestFileReader::currentSection{};
vector<Continent *> ConquestFileReader::continentsList{};
vector<Territory *> ConquestFileReader::territoriesList{};
map<string, Continent*> ConquestFileReader::nameToContinentMap{};
map<string, Territory*> ConquestFileReader::nameToTerritoryMap{};
map<Territory*, set<string>> ConquestFileReader::adjacentTerritoryNamesMap{};


ConquestFileReader::ConquestFileReader(const ConquestFileReader &original) {}

ConquestFileReader &ConquestFileReader::operator=(const ConquestFileReader &original) = default;

ostream &operator<<(ostream &os, const ConquestFileReader &reader) {
    return os << "ConquestFileReader: [continentList size =" << ConquestFileReader::continentsList.size()
              << ", territoriesList size = " << ConquestFileReader::territoriesList.size() << "]";
}

ConquestFileReader::~ConquestFileReader() = default;

Map *ConquestFileReader::loadConquestMap(const string &mapName) {
    // Have a clear setup when loading a new map
    continentsList.clear();
    territoriesList.clear();
    nameToContinentMap.clear();
    nameToTerritoryMap.clear();
    adjacentTerritoryNamesMap.clear();

    try {
        // Read map
        fstream mapFile;
        MapLoader::checkPattern(mapName, MAP_FILENAME_FORMAT_REGEX);

        mapFile.open(MAP_DIRECTORY + mapName, ios::in);

        if (mapFile.is_open()) {
            parseFile(mapFile);
            mapFile.close();
        } else
            MapLoader::throwInvalidMapException();

        // Construct Map object
        constructAdjencyList();
        auto *graph = new Map(territoriesList, continentsList);

        return graph;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
        return nullptr;
    }
}

void ConquestFileReader::parseFile(fstream &mapFile) {
    string line;
    int continentId = 1;
    int territoryId = 1;

    while (getline(mapFile, line)) {
        line.erase(line.find_last_not_of(" \n\r\t") + 1);
        if (line.empty() || line.at(0) == ';')
            continue;

        bool isUpdated = updateSection(line);

        if (!isUpdated) {
            if (currentSection == continents) {
                MapLoader::checkPattern(line, CONTINENT_REGEX);
                continentsList.push_back(createContinents(line, continentId));
                continentId++;
            } else if (currentSection == territories) {
                MapLoader::checkPattern(line, TERRITORY_REGEX);
                territoriesList.push_back(createTerritories(line, territoryId));
                territoryId++;
            }
        }
    }
}

bool ConquestFileReader::updateSection(string &line) {
    if (line.at(0) == '[') {
        if (line == "[Continents]") {
            currentSection = continents;
        } else if (line == "[Territories]") {
            currentSection = territories;
        } else {
            currentSection = other;
        }
        return true;
    }
    return false;
}

Continent *ConquestFileReader::createContinents(const string &line, int &continentId) {
    const char *token = strtok((char *) line.c_str(), "=");
    int counter = 0;
    auto *continent = new Continent(); // must create with new operator or else will be deleted at end of the method
    while (token != nullptr) {
        if (counter == 0) {
            continent->setContinentName(token);
        } else if (counter == 1) {
            continent->setBonus(atoi(token));
        }
        token = strtok(nullptr, "=");
        counter++;
    }

    continent->setContinentId(continentId);
    nameToContinentMap[continent->getContinentName()] = continent;

    return continent;
}

Territory *ConquestFileReader::createTerritories(const string &line, int &territoryId) {
    if (continentsList.empty()) MapLoader::throwInvalidMapException();

    const char *token = strtok((char *) line.c_str(), ",");
    int counter = 0;
    auto *territory = new Territory();
    while (token != nullptr) {
        if (counter == 0) {
            territory->setTerritoryName(token);
            nameToTerritoryMap[token] = territory;
        } else if (counter == 3) {
            Continent* continent = nameToContinentMap[token];
            if(continent == nullptr) MapLoader::throwInvalidMapException();

            territory->setContinentId(continent->getContinentId());
            continent->getTerritories().push_back(territory);
        } else if (counter >= 4) {
            adjacentTerritoryNamesMap[territory].insert(token);
        }
        token = strtok(nullptr, ",");
        counter++;
    }

    territory->setTerritoryId(territoryId);

    return territory;
}

void ConquestFileReader::constructAdjencyList() {
    for (pair<Territory *, set<string>> pair : adjacentTerritoryNamesMap)
        for(const string& territoryName : pair.second)
            pair.first->getAdjList().push_back(nameToTerritoryMap[territoryName]);
}

// ================================ ConquestFileReaderAdapter  =========================================================
ConquestFileReaderAdapter::ConquestFileReaderAdapter(): conquestFileReader(new ConquestFileReader()) {}

ConquestFileReaderAdapter::ConquestFileReaderAdapter(const ConquestFileReaderAdapter &original): conquestFileReader(new ConquestFileReader(*original.conquestFileReader)) {}

ConquestFileReaderAdapter &ConquestFileReaderAdapter::operator=(const ConquestFileReaderAdapter &original) {
    if(&original != this){
        delete conquestFileReader;
        conquestFileReader = new ConquestFileReader(*original.conquestFileReader);
    }
    return *this;
}

ostream &operator<<(ostream &os, const ConquestFileReaderAdapter &reader) { return os << "Conquest File Reader Adapter";}

ConquestFileReaderAdapter::~ConquestFileReaderAdapter() {
    delete conquestFileReader;
    conquestFileReader = nullptr;
}

Map *ConquestFileReaderAdapter::loadMap(const string &mapName) {
    return conquestFileReader->loadConquestMap(mapName);
}