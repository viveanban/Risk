#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <algorithm>
#include "Map.h"

using namespace std;

/**
 * Territory Class implementation
 */

Territory::Territory() : territoryName(), territoryId(), unitNbr(), continentId(), owner(), adjList() {}

Territory::Territory(const Territory &original) {
    territoryName = original.territoryName;
    owner = original.owner;
    continentId = original.continentId;
    territoryId = original.territoryId;
    unitNbr = original.unitNbr;
    adjList = vector<int>(original.adjList.size());
    for (int i = 0; i < adjList.size(); i++)
        adjList[i] = original.adjList[i];
}

Territory &Territory::operator=(const Territory &otherTerritory) {
    owner = otherTerritory.owner;
    continentId = otherTerritory.continentId;
    territoryId = otherTerritory.territoryId;
    unitNbr = otherTerritory.unitNbr;
    adjList = vector<int>(otherTerritory.adjList.size());
    for (int i = 0; i < adjList.size(); i++)
        adjList[i] = otherTerritory.adjList[i];

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Territory &t) {
    return stream << "\tInformation on Territory object:" << endl <<
                  "\tTerritory Name: " << t.getTerritoryName() << endl <<
                  "\tTerritory ID: " << t.getTerritoryId() <<
                  "\tContinent id it belongs to: " << t.getContinentId() << endl <<
                  "\tNumber of adjacent Territories: " << t.getAdjList().size() << endl <<
                  "\tNumber of unit deployed on territory: " << t.getUnitNbr() << endl <<
                  "\tOwner of the Territory: " << t.getOwner() << endl;
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
}

int Territory::getContinentId() {
    return this->continentId;
}

void Territory::setContinentId(int continentId) {
    this->continentId = continentId;
}

Player* Territory::getOwner() {
    return this->owner;
}

void Territory::setOwner(Player *owner) {
    this->owner = owner;
}

void Territory::addLink(int t) {
    adjList.push_back(t);
}

vector<int> &Territory::getAdjList() {
    return this->adjList;
}

void Territory::setAdjList(vector<int> &adjList) {
    this->adjList = adjList;

}

/**
 * Graph Class implementation
 */
Graph::Graph() : territoryList() {}

Graph::Graph(vector<Territory *> &territoryList, vector<Continent *> &continentList) : territoryList(territoryList),
                                                                                       continentList(continentList) {}

Graph::Graph(const Graph &original) {
    territoryList = vector<Territory *>(original.territoryList.size());
    for (int i = 0; i < territoryList.size(); i++)
        territoryList[i] = new Territory(*original.territoryList[i]);
    continentList = vector<Continent *>(original.continentList.size());
    for (int i = 0; i < continentList.size(); i++)
        continentList[i] = new Continent(*original.continentList[i]);
}

Graph &Graph::operator=(const Graph &otherGraph) {
    territoryList = vector<Territory *>(otherGraph.territoryList.size());
    for (int i = 0; i < territoryList.size(); i++)
        territoryList[i] = new Territory(*otherGraph.territoryList[i]);
    continentList = vector<Continent *>(otherGraph.continentList.size());
    for (int i = 0; i < continentList.size(); i++)
        continentList[i] = new Continent(*otherGraph.continentList[i]);
    return *this;
}

vector<Territory *> &Graph::getTerritoryList() {
    return this->territoryList;
}

std::ostream &operator<<(std::ostream &stream, Graph &g) {
    return stream << "\tInformation on Graph object:" << endl <<
                  "\tNumber of Territories: " << g.getTerritoryList().size() << endl <<
                  "\tNumber of Continents: " << g.getContinentList().size() << endl;
}

void Graph::setTerritoryList(vector<Territory *> &territoryList) {
    this->territoryList = territoryList;
}

void Graph::addTerritory(Territory *territory) {
    territoryList.push_back(territory);
}

vector<Continent *> &Graph::getContinentList() {
    return this->continentList;
}

void Graph::setContinentList(vector<Continent *> &continentList) {
    this->continentList = continentList;
}

void Graph::addContinent(Continent *continent) {
    continentList.push_back(continent);
}

bool Graph::isGraphConnected() {
    set<int> seenTerritories{};
    stack<int> toVisitStack;

    toVisitStack.push(getTerritoryList().at(0)->getTerritoryId());

    while (!toVisitStack.empty()) {

        //pop the top territories to visit it
        int currentTerritory = toVisitStack.top();
        toVisitStack.pop();

        //add all unvisited territories to the stack to visit them later
        for (int territory : getTerritoryById(currentTerritory)->getAdjList()) {
            if (seenTerritories.count(territory) == 0) {
                toVisitStack.push(territory);
                seenTerritories.insert(territory);
            }
        }
    }

    //once we don't have anymore territories to visit in the stack,
    // we need to verify if we visited all territories
    if (seenTerritories.size() == getTerritoryList().size()) {
        return true;
    } else {
        return false;
    }
}

bool Graph::isContinentSubgraphConnected() {
    set<int> visitedContinents{};
    set<int> seenTerritories;
    stack<int> territoriesToVisit;
    int numberOfContinents = getContinentList().size();
    // starting from the first territory
    territoriesToVisit.push(getTerritoryList().at(0)->getTerritoryId());
    // loop until all continents visited or until no more connected territories to visit
    while (!territoriesToVisit.empty() && visitedContinents.size() < numberOfContinents) {
        int currentTerritoryId = territoriesToVisit.top();
        Territory *currentTerritory = getTerritoryById(currentTerritoryId);
        territoriesToVisit.pop();

        for (int territory : currentTerritory->getAdjList()) {
            // if we see the territory for the first time, add it to the territories to visit
            if (seenTerritories.count(territory) == 0) {
                territoriesToVisit.push(territory);
                seenTerritories.insert(territory);
                // add continent to the set of visitedContinents
                visitedContinents.insert(currentTerritory->getContinentId());
            }
        }
    }
    if (visitedContinents.size() == numberOfContinents) {
        return true;
    } else {
        return false;
    }
}

bool Graph::isTerritoryContinentUnique() {

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

bool Graph::validate() {
    bool connectedTerritories = isGraphConnected();
    bool connectedContinents = isContinentSubgraphConnected();
    bool uniqueTerritories = isTerritoryContinentUnique();

    cout << boolalpha << "Territories: " << connectedTerritories << ", Continents: " << connectedContinents
         << ", Unique Territories: " << uniqueTerritories << endl;
    return
            connectedTerritories &&
            connectedContinents &&
            uniqueTerritories;
}

Graph::~Graph() {
    cout << "Deleting Graph..." << endl;
    cout << "Deleting Continents" << endl;
    for (auto continent: continentList) {
        delete continent;
        continent = nullptr;
    }
    continentList.clear();

    cout << "Deleting Territories" << endl;
    for (auto territory: territoryList) {
        delete territory;
        territory = nullptr;
    }
    territoryList.clear();
}

Territory *Graph::getTerritoryById(int id) {
    for (Territory *t: territoryList) {
        if (t->getTerritoryId() == id)
            return t;
    }
    return nullptr;
}

/**
 * Continent Class implementation
 */
Continent::Continent() : continentId(), continentName(),
                         territories(), bonus() {}

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
    territories = vector<Territory *>(otherContinent.territories.size());
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*otherContinent.territories[i]);

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Continent &c) {
    return stream << "\tInformation on Continent object:" << endl <<
                  "\tName: " << c.getContinentName() << endl <<
                  "\tId: " << c.getContinentId() << endl <<
                  "\tNumber of countries in it: " << c.getTerritories().size() << endl <<
                  "\tOwner owning continent (if any): " << c.getOwner() << endl;
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

vector<Territory *> &Continent::getTerritories() {
    return this->territories;
}

bool Continent::isSameOwner() {
    set<Territory *> setOfTerritoriesInContinent;
    for (Territory *territory : getTerritories()) {
        setOfTerritoriesInContinent.insert(territory);
    }
    return setOfTerritoriesInContinent.size() == 1;
}

string Continent::getOwner() {

//    if (isSameOwner()) {
//        return getTerritories().at(0)->getOwner().getPlayerName();
//    }
    return "";
}

void Continent::setTerritories(vector<Territory *> territories) {
    this->territories = territories;
}