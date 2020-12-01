#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <regex>
#include <set>
#include "ConquestFileReader.h"
#include "MapLoader.h"

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

ConquestFileReader &ConquestFileReader::operator=(const ConquestFileReader &original) { return *this; }

ConquestFileReader::~ConquestFileReader() {}

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
    } catch (const invalid_argument e) {
        cout << e.what() << endl;
        return NULL;
    }
    return nullptr;
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
        if (line.compare("[Continents]") == 0) {
            currentSection = continents;
        } else if (line.compare("[Territories]") == 0) {
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
            if(continent == nullptr)
                MapLoader::throwInvalidMapException();
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