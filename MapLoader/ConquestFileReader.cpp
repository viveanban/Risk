#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <regex>
#include "ConquestFileReader.h"

const string ConquestFileReader::MAP_FILENAME_FORMAT_REGEX = "[^.]+\\.+map";
const string ConquestFileReader::MAP_DIRECTORY = "../maps/conquest_maps/";
const string ConquestFileReader::CONTINENT_REGEX = "(([A-Z]([a-z])*\\s*)+=[0-9]*)+";
const string ConquestFileReader::TERRITORY_REGEX = "((([A-Z][a-z]*\\s*)+),[0-9]*,[0-9]*(,((([A-Z][a-z]*\\s*)+))*)*)";

ConquestFileReader::Section ConquestFileReader::currentSection{};
vector<Continent *> ConquestFileReader::continentsList{};
vector<Territory *> ConquestFileReader::territoriesList{};


ConquestFileReader::ConquestFileReader(const ConquestFileReader &original) {}

ConquestFileReader &ConquestFileReader::operator=(const ConquestFileReader &original) { return *this; }

ConquestFileReader::~ConquestFileReader() {}

Map *ConquestFileReader::loadConquestMap(const string &mapName) {
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
//                checkPattern(line, CONTINENT_REGEX); TODO: Uncomment after regex is determined
                continentsList.push_back(createContinents(line, continentId));
                continentId++;
            } else if (currentSection == territories) {
//                checkPattern(line, TERRITORY_REGEX); TODO: Uncomment after regex is determined
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

    //TODO: Create unordered list to map continent name to id
    continent->setContinentId(continentId);

    return continent;
}

Territory *ConquestFileReader::createTerritories(const string &line, int &territoryId) {
    if (continentsList.empty()) throwInvalidMapException();

    const char *token = strtok((char *) line.c_str(), ",");
    int counter = 0;
    auto *territory = new Territory();
    while (token != nullptr) {
        if (counter == 0) {
            // TODO: Add territory name + ID in a hash map
            territory->setTerritoryName(token);
        } else if (counter == 3) {
            //TODO: Get the id mapped to the continent name
//            territory->setContinentId(id);
        } else if (counter >= 4) {
            //TODO: Add key value in a hash map with the territory object mapped to a list of territory names (we don't have the ids yet)
        }
        token = strtok(nullptr, ",");
        counter++;
    }

    // TODO: Add the created territory to the territory list of the it's corresponding continent object
    territory->setTerritoryId(territoryId);

    return territory;
}


// TODO: Ask TA if it is ok to call a method from the original map loader (some of the conquest file reader depends on methods defined in MapLoader)
void ConquestFileReader::checkPattern(const string &line, const string &pattern) {
    if (!regex_match(line, regex(pattern))) {
        throwInvalidMapException();
    }
}

void ConquestFileReader::throwInvalidMapException() {
    throw invalid_argument("Error loading map, this map format is invalid!");
}