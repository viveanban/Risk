//
// Created by Viveka Anban on 24-09-2020.
//

#ifndef RISK_MAPLOADER_H
#define RISK_MAPLOADER_H

#include <string>
using std::string;
using std::ifstream;

namespace mapLoader {
    class MapLoader {
    public:
        MapLoader();
        void readMap(string file_path);
    private:
        bool parseFile(ifstream mapFile);
    };
}
#endif //RISK_MAPLOADER_H
