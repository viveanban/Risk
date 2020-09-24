//
// Created by Viveka Anban on 24-09-2020.
// readFile: https://www.w3schools.com/cpp/cpp_files.asp
//
#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using std::ios;
using std::cout;
using std::endl;
using std::ifstream;

namespace mapLoader {
    MapLoader::MapLoader() {
        cout << "MapLoader Ctor" << endl;
    }

    bool parseFile(ifstream ifstream);

    void MapLoader::readMap(string file_path) {
        ifstream mapFile(file_path);

        if (mapFile.is_open()) {  //checking whether the file is open
            parseFile(mapFile);
            mapFile.close(); //close the file object.
        }
    }

    // think about each section 1 by 1
    bool parseFile(ifstream mapFile) {
        string line;
        while(getline(mapFile, line)){ //read data from file object and put it into string.
            const char* str = line.c_str();
            // Returns first token
            char *token = strtok((char*)str, " ");

            // Keep printing tokens while one of the
            // delimiters present in str[].
            while (token != NULL)
            {
                printf("%s\n", token);
                token = strtok(NULL, " ");
            }
        }
        return false;
    }
}


