#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include "Map.h"
#include "../Player/Player.h"

using namespace std;

/**
 * Territory Class implementation
 */

Territory::Territory() : territoryName(), territoryId(), unitNbr(), continentId(), owner(nullptr), adjList(), priority() {}

Territory::Territory(const Territory &original) {
    territoryName = original.territoryName;
    owner = original.owner;
    continentId = original.continentId;
    territoryId = original.territoryId;
    unitNbr = original.unitNbr;
    adjList = vector<Territory *>(original.adjList.size());
    // Note: Shallow copy done on purpose. We want the new territory's adjacent list to be composed of pointers pointing
    // to the same territories as those pointed by the adjacent list of original
    for (int i = 0; i < adjList.size(); i++)
        adjList[i] = original.adjList[i];
}

Territory &Territory::operator=(const Territory &otherTerritory) {
    territoryName = otherTerritory.territoryName;
    owner = otherTerritory.owner;
    continentId = otherTerritory.continentId;
    territoryId = otherTerritory.territoryId;
    unitNbr = otherTerritory.unitNbr;
    adjList = vector<Territory *>(otherTerritory.adjList.size());
    // Note: Shallow copy done on purpose. We want the new territory's adjacent list to be composed of pointers pointing
    // to the same territories as those pointed by the adjacent list of otherTerritory
    for (int i = 0; i < adjList.size(); i++)
        adjList[i] = otherTerritory.adjList[i];

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Territory &t) {
    return stream << "Information on Territory object:" << endl
                  << "Address: " << &t << endl
                  << "Territory Name: " << t.getTerritoryName() << endl
                  << "Territory ID: " << t.getTerritoryId() << endl
                  << "Continent id it belongs to: " << t.getContinentId() << endl
                  << "Number of adjacent Territories: " << t.getAdjList().size() << endl
                  << "Number of unit deployed on territory: " << t.getUnitNbr() << endl
                  << "Owner of the Territory: " << t.getOwner() << endl;
}

string Territory::getTerritoryName() {
    return this->territoryName;
}

void Territory::setTerritoryName(string territoryName) {
    this->territoryName = territoryName;
}

int Territory::getTerritoryId() {
    return this->territoryId;
}

void Territory::setTerritoryId(int territoryId) {
    this->territoryId = territoryId;
}

int Territory::getUnitNbr() {
    return this->unitNbr;
}

void Territory::setUnitNbr(int unitNbr) {
    this->unitNbr = unitNbr;
    this->priority = unitNbr;
}

int Territory::getContinentId() {
    return this->continentId;
}

void Territory::setContinentId(int continentId) {
    this->continentId = continentId;
}

Player *Territory::getOwner() {
    return this->owner;
}

void Territory::setOwner(Player *newOwner) {
    if (this->owner != nullptr)
        this->owner->removeTerritory(this);

    this->owner = newOwner;
    newOwner->addTerritory(this);
}

vector<Territory *> &Territory::getAdjList() {
    return this->adjList;
}

int Territory::getPriority() const {
    return priority;
}

void Territory::setPriority(int priority) {
    Territory::priority = priority;
}

/**
 * Continent Class implementation
 */
Continent::Continent() : continentId(), continentName(), territories(), bonus() {}

Continent::~Continent() {
    for(auto territory: territories) {
        cout << "Deleting territory" << endl;
        delete territory;
        territory = nullptr;
    }
    territories.clear();
}

Continent::Continent(const Continent &original) {
    continentId = original.continentId;
    bonus = original.bonus;
    continentName = original.continentName;
    territories = vector<Territory *>(original.territories.size());
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*original.territories[i]);
}

Continent &Continent::operator=(const Continent &otherContinent) {
    continentName = otherContinent.continentName;
    continentId = otherContinent.continentId;
    bonus = otherContinent.bonus;

    if(!territories.empty()) {
        for (Territory* territory: territories)
            delete territory;

        territories.clear();
    }

    territories = vector<Territory *>(otherContinent.territories.size());
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*otherContinent.territories[i]);

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Continent &c) {
    return stream << "Information on Continent object:" << endl
                  << "Name: " << c.getContinentName() << endl
                  << "Id: " << c.getContinentId() << endl
                  << "Number of countries in it: " << c.getTerritories().size() << endl
                  << "Owner owning continent (if any): " << c.getOwner() << endl;
}

int Continent::getContinentId() {
    return this->continentId;
}

void Continent::setContinentId(int continentId) {
    this->continentId = continentId;
}

string Continent::getContinentName() {
    return this->continentName;
}

void Continent::setContinentName(string continentName) {
    this->continentName = continentName;
}

int Continent::getBonus() {
    return this->bonus;
}

void Continent::setBonus(int bonus) {
    this->bonus = bonus;
}

bool Continent::isSameOwner() {
    set<Player *> players;
    for (Territory *territory : getTerritories()) {
        if(territory->getOwner() == nullptr)
            return false;
        else
            players.insert(territory->getOwner());
    }
    return players.size() == 1;
}

Player *Continent::getOwner() {
    return isSameOwner() ? (*getTerritories().at(0)).getOwner() : nullptr;
}

vector<Territory *> &Continent::getTerritories() {
    return this->territories;
}

/**
 * Map Class implementation
 */
Map::Map() : territoryList(), continentList() {}

Map::Map(vector<Territory *> &territoryList, vector<Continent *> &continentList) : territoryList(territoryList),
                                                                                       continentList(continentList) {}
Map::~Map() {
    cout << "Deleting Map..." << endl;
    for (auto continent: continentList) {
        cout << "Deleting Continent" << endl;
        delete continent;
        continent = nullptr;
    }
    continentList.clear();
}

// Note: Shallow copy for territoryList done purposefully. The Map is not responsible to create the new territories.
// The Continent is.
Map::Map(const Map &original) {
    territoryList = vector<Territory *>(original.territoryList.size());
    for (int i = 0; i < territoryList.size(); i++)
        territoryList[i] = original.territoryList[i];
    continentList = vector<Continent *>(original.continentList.size());
    for (int i = 0; i < continentList.size(); i++)
        continentList[i] = new Continent(*original.continentList[i]);
}

// Note: Shallow copy for territoryList done purposefully. The Map is not responsible to create the new territories.
// The Continent is.
Map &Map::operator=(const Map &otherMap) {
    territoryList = vector<Territory *>(otherMap.territoryList.size());
    for (int i = 0; i < territoryList.size(); i++)
        territoryList[i] = otherMap.territoryList[i];

    if(!continentList.empty()) {
        for (Continent* continent: continentList)
            delete continent;

        continentList.clear();
    }

    continentList = vector<Continent *>(otherMap.continentList.size());
    for (int i = 0; i < continentList.size(); i++)
        continentList[i] = new Continent(*otherMap.continentList[i]);

    return *this;
}

vector<Territory *> &Map::getTerritoryList() {
    return this->territoryList;
}

std::ostream &operator<<(std::ostream &stream, Map &g) {
    return stream << "Information on Map object:" << endl
                  << "Number of Territories: " << g.getTerritoryList().size() << endl
                  << "Number of Continents: " << g.getContinentList().size() << endl;
}

vector<Continent *> &Map::getContinentList() {
    return this->continentList;
}

bool Map::isMapConnected() {
    set<int> seenTerritories{};
    stack<int> toVisitStack;

    toVisitStack.push(getTerritoryList().at(0)->getTerritoryId());

    while (!toVisitStack.empty()) {

        // Pop the top territories to visit it
        int currentTerritory = toVisitStack.top();
        toVisitStack.pop();

        // Add all unvisited territories to the stack to visit them later
        for (Territory * territory : getTerritoryById(currentTerritory)->getAdjList()) {
            if (seenTerritories.count(territory->getTerritoryId()) == 0) {
                toVisitStack.push(territory->getTerritoryId());
                seenTerritories.insert(territory->getTerritoryId());
            }
        }
    }

    // Once we don't have anymore territories to visit in the stack,
    // we need to verify if we visited all territories
    if (seenTerritories.size() == getTerritoryList().size()) {
        return true;
    } else {
        return false;
    }
}

bool Map::isContinentSubgraphConnected() {
    set<int> visitedContinents{};
    set<int> seenTerritories;
    stack<int> territoriesToVisit;
    int numberOfContinents = getContinentList().size();
    // Starting from the first territory
    territoriesToVisit.push(getTerritoryList().at(0)->getTerritoryId());
    // Loop until all continents visited or until no more connected territories to visit
    while (!territoriesToVisit.empty() && visitedContinents.size() < numberOfContinents) {
        int currentTerritoryId = territoriesToVisit.top();
        Territory *currentTerritory = getTerritoryById(currentTerritoryId);
        territoriesToVisit.pop();
        visitedContinents.insert(currentTerritory->getContinentId());
        seenTerritories.insert(currentTerritory->getTerritoryId());


        for (Territory * territory : currentTerritory->getAdjList()) {
            // If we see the territory for the first time, add it to the territories to visit
            if (seenTerritories.count(territory->getTerritoryId()) == 0) {
                territoriesToVisit.push(territory->getTerritoryId());
                seenTerritories.insert(territory->getTerritoryId());
                // Add continent to the set of visitedContinents
                visitedContinents.insert(territory->getContinentId());
            }
        }
    }
    if (visitedContinents.size() == numberOfContinents) {
        return true;
    } else {
        return false;
    }
}

bool Map::isTerritoryContinentUnique() {
    set<string> seenTerritoryNames{};
    set<int> seenTerritoryIds{};

    for (Continent *continent : getContinentList()) {
        for (Territory *territory: (*continent).getTerritories()) {
            if (seenTerritoryNames.count(territory->getTerritoryName()) == 0 &&
                seenTerritoryIds.count(territory->getTerritoryId()) == 0) {
                seenTerritoryNames.insert(territory->getTerritoryName());
                seenTerritoryIds.insert(territory->getTerritoryId());
            } else
                return false;
        }
    }

    return seenTerritoryNames.size() == getTerritoryList().size();
}

bool Map::validate() {
    bool connectedTerritories = isMapConnected();
    bool connectedContinents = isContinentSubgraphConnected();
    bool uniqueTerritories = isTerritoryContinentUnique();
    cout << "Validating Map..." << endl;
    cout << boolalpha
         << "All Territories are connected: " << connectedTerritories
         << ", All Continents are connected: " << connectedContinents
         << ", All Territories are unique: " << uniqueTerritories << endl;
    bool isValid = connectedTerritories &&
                   connectedContinents &&
                   uniqueTerritories;
    if (isValid) {
        cout << "The Map is valid!" << endl;
    } else {
        cout << "The Map is not valid!" << endl;
    }
    return isValid;
}

Territory *Map::getTerritoryById(int id) {
    for (Territory *t: territoryList) {
        if (t->getTerritoryId() == id)
            return t;
    }
    return nullptr;
}