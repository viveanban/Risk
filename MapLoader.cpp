//
// Created by Viveka Anban on 24-09-2020.
// readFile: https://www.w3schools.com/cpp/cpp_files.asp
//
#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string.h>

using std::ios;
using std::cout;
using std::endl;
using std::ifstream;

//class MapLoader {
//    static Map load(string mapFileName) {
//
//    }
//};

namespace mapLoader {

//    TODO: Regroup these enums under a class FileTemplate
    enum Section {
        other, continents, countries, borders
    };
    enum ContinentEntry {
        continentName, bonus
    };
    enum CountryEntry {
        id, countryName, continentId
    };
    enum BorderEntry {
        countryId, adjacentCountries
    };

    Section currentSection;

    // TODO: Temperoray vectors where the continents, countries and borders are created
    vector<MapLoader::Continent> continentList;
    vector<MapLoader::Country> countryList;
    vector<MapLoader::Borders> bordersList;

    MapLoader::MapLoader() {
        cout << "MapLoader Ctor" << endl;
    }

    void MapLoader::readMap(string file_path) {
        fstream mapFile;
        mapFile.open(file_path, ios::in | ios::binary);

        if (mapFile.is_open()) {  //checking whether the file is open
            parseFile(mapFile);
            mapFile.close(); //close the file object.
        }
    }

    bool MapLoader::parseFile(fstream &mapFile) {
        string line;
        skipIrrelevantLines(mapFile, line);

        while (getline(mapFile, line)) {
            std::replace(line.begin(), line.end(), '\r', ' ');
            bool isUpdated = updateCategory(line);

            // Line tokenizing will only happen in the content of relevant sections
            if (!isUpdated) {
                if (currentSection == continents) {
                    continentList.push_back(createContinentFromLine(line));
                } else if (currentSection == countries) {
                    countryList.push_back(createCountryFromLine(line));
                } else if (currentSection == borders) {
                    bordersList.push_back(createBordersFromLine(line));
                }
            }
        }
        return false;
    }

    void MapLoader::skipIrrelevantLines(fstream &mapFile, string &line) {
        //read all lines until you get to the beginning of a section content (section title has been read)
        while (getline(mapFile, line) && !isSectionRelevant(line));
    }

    bool MapLoader::isSectionRelevant(string &line) {
        updateCategory(line);
        return currentSection != other;
    }

    bool MapLoader::updateCategory(string &line) {
        if (!line.empty() && line.at(0) == '[') {
            if (line.compare("[continents]\r") == 0) {
                currentSection = continents;
            } else if (line.compare("[countries]\r") == 0) {
                currentSection = countries;
            } else if (line.compare("[borders]\r") == 0) {
                currentSection = borders;
            } else {
                currentSection = other;
            }
            return true;
        }
        return false;
    }

    MapLoader::Continent MapLoader::createContinentFromLine(const string &line) {
        const char *token = strtok((char *) line.c_str(), " ");
        int counter = 0;
        Continent continent;
        while (token != NULL) {
            if (counter == continentName) {
                continent.name = token;
            } else if (counter == bonus) {
                long convertedToken;
                continent.bonus = getInt(token, convertedToken);
            }

            token = strtok(NULL, " ");
            counter++;
        }
        return continent;
    }

    MapLoader::Country MapLoader::createCountryFromLine(const string &line) {
        const char *token = strtok((char *) line.c_str(), " ");
        int counter = 0;
        Country country;
        while (token != NULL) {
            long convertedToken;
            if (counter == id) {
                convertedToken = getInt(token, convertedToken);
                country.id = convertedToken;
            } else if (counter == countryName) {
                country.name = token;
            } else if (counter == continentId) {
                country.continentId = getInt(token, convertedToken);
            }

            token = strtok(NULL, " ");
            counter++;
        }
        return country;
    }

    MapLoader::Borders MapLoader::createBordersFromLine(const string &line) {
        const char *token = strtok((char *) line.c_str(), " ");
        int counter = 0;
        Borders borders;
        while (token != NULL) {
            long int convertedToken;
            if (counter == countryId) {
                convertedToken = getInt(token, convertedToken);
                borders.countryId = convertedToken;
            } else if (counter >= adjacentCountries) {
                convertedToken = getInt(token, convertedToken);
                borders.adjacentCountries.push_back(convertedToken);
            }
            token = strtok(NULL, " ");
            counter++;
        }
        return borders;
    }

    long MapLoader::getInt(const char *token, long &convertedToken) {
        char *endPointer;
//        std::replace(token.)
        convertedToken = strtol(token, &endPointer, 10);
        if (*endPointer) {
            //conversion failed because the input was not a number
            cout << "Conversion failed because the input was not a number" << endl;
            return -1;
        } else {
            return convertedToken;
        }
    }

}


