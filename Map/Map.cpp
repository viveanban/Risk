#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include "Map.h"

using namespace std;

/**
 * Territory Class implementation
 */

Territory::Territory() : territoryName(nullptr), territoryId(nullptr), unitNbr(nullptr), continentId(nullptr), owner(nullptr),
                         adjList(nullptr) {}

string Territory::getTerritoryName() {
    return *territoryName;
}

void Territory::setTerritoryName(string *territoryName) {
    this->territoryName = territoryName;
}

int Territory::getTerritoryId() {
    return *this->territoryId;
}

void Territory::setTerritoryId(int territoryId) {
    *this->territoryId = territoryId;
}

int Territory::getUnitNbr() {
    return *unitNbr;
}

void Territory::setUnitNbr(int *unitNbr) {
    this->unitNbr = unitNbr;
}

int Territory::getContinentId() {
    return *this->continentId;
}

void Territory::setContinentId(int continentId) {
    *this->continentId = continentId;
}

string Territory::getOwner() {
    return *owner;
}

void Territory::setOwner(string *owner) {
    Territory::owner = owner;
}

void Territory::addLink(Territory *t) {
    (*adjList).push_back(t);
}

vector<Territory *> & Territory::getAdjList() {
    return *adjList;
}

/**
 * Graph Class implementation
 */
Graph::Graph() : territoryList(nullptr) {}

Graph::Graph(vector<Territory *> *territoryList) : territoryList(territoryList) {}

vector<Territory *> Graph::getTerritoryList() {
    return *territoryList;
}

void Graph::setTerritoryList(vector<Territory *> *territoryList) {
    this->territoryList = territoryList;
}

void Graph::addTerritory(Territory *territory) {
    territoryList->push_back(territory);
}

bool Graph::isGraphConnected() {

    map<Territory *, bool> territories;
    stack<Territory *> toVisitStack;
    toVisitStack.push(getTerritoryList().at(0));

    for (Territory *territory: getTerritoryList()) {
        territories.insert(pair<Territory *, bool>(territory, false));
    }

    while (!toVisitStack.empty()) {

        Territory *currentTerritory = toVisitStack.top();
        toVisitStack.pop();

        for (Territory *territory : (*currentTerritory).getAdjList()) {
            if (territories[territory] == false) {
                toVisitStack.push(territory);
            }
        }

        for (Territory *territory: getTerritoryList()) {
            if (territories[territory] == false)
                return false;
        }
        return true;
    }

    return true;
}

bool Graph::isContinentSubgraphConnected() {
    return true;

}

bool Graph::isCountryContinentOneToOne() {
    return true;
}

bool Graph::validate() {
    return
            isGraphConnected() &&
            isContinentSubgraphConnected() &&
            isCountryContinentOneToOne();
}

/**
 * Continent Class implementation
 */
Continent::Continent() : continentId(nullptr), continentName(nullptr), territoriesInContinent(nullptr), bonus(nullptr) {}

int Continent::getContinentId() {
    return *continentId;
}

void Continent::setContinentId(int continentId) {
    *this->continentId = continentId;
}

string Continent::getContinentName() {
    return *continentName;
}

void Continent::setContinentName(string *continentName) {
    this->continentName = continentName;
}

int Continent::getBonus() {
    return *bonus;
}

void Continent::setBonus(int *bonus) {
    this->bonus = bonus;
}

vector<Territory *> & Continent::getTerritoriesInContinent() {
    return *territoriesInContinent;
}

void Continent::setTerritoriesInContinent(vector<Territory *> *territoriesInContinent) {
    this->territoriesInContinent = territoriesInContinent;
}

void Continent::addTerritoryInContinent(Territory *n) {
    (*territoriesInContinent).push_back(n);
}

bool Continent::isSameOwner() {
    set<Territory *> setOfTerritoriesInContinent;
    for (Territory *territory : getTerritoriesInContinent()) {
        setOfTerritoriesInContinent.insert(territory);
    }
    return setOfTerritoriesInContinent.size() == 1;
}

string Continent::getOwner() {

    if (isSameOwner()) {
        return getTerritoriesInContinent().at(0)->getOwner();
    }
}
