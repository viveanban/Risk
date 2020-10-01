#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include "Map.h"

using namespace std;

/**
 * Territory Class implementation
 */

Territory::Territory() : territoryName(), territoryId(), unitNbr(), continentId(), owner(),
                         adjList() {}

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

string Territory::getOwner() {
    return this->owner;
}

void Territory::setOwner(string owner) {
    this->owner = owner;
}

void Territory::addLink(Territory *t) {
    adjList.push_back(t);
}

vector<Territory *> &Territory::getAdjList() {
    return this->adjList;
}

Territory::Territory(const Territory &original) {
    territoryName = original.territoryName;
    owner = original.owner;
    continentId = original.continentId;
    territoryId = original.territoryId;
    unitNbr = original.unitNbr;
    adjList = original.adjList;
}

/**
 * Graph Class implementation
 */
Graph::Graph() : territoryList() {}

Graph::Graph(vector<Territory *> &territoryList, vector<Continent *> &continentList) : territoryList(territoryList),
                                                                                       continentList(continentList) {}

vector<Territory *> &Graph::getTerritoryList() {
    return this->territoryList;
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
    set<Territory *> seenTerritories{};
    stack<Territory *> toVisitStack;

    toVisitStack.push(getTerritoryList().at(0));

    while (!toVisitStack.empty()) {

        //pop the top territories to visit it
        Territory *currentTerritory = toVisitStack.top();
        toVisitStack.pop();

        //add all unvisited territories to the stack to visit them later
        for (Territory *territory : (*currentTerritory).getAdjList()) {
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
    set<Territory *> seenTerritories;
    stack<Territory *> territoriesToVisit;
    int numberOfContinents = getContinentList().size();
    // starting from the first territory
    territoriesToVisit.push(getTerritoryList().at(0));
    // loop until all continents visited or until no more connected territories to visit
    while (!territoriesToVisit.empty() && visitedContinents.size() < numberOfContinents) {
        Territory *currentTerritory = territoriesToVisit.top();
        territoriesToVisit.pop();

        for (Territory *territory: currentTerritory->getAdjList()) {
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

bool Graph::isTerritoryContinentOneToOne() {

    set<Territory *> seenTerritories {};

    for(Continent* continent : getContinentList()){
        for(Territory* territory: (*continent).getTerritories()){
            if(seenTerritories.count(territory) == 0)
                seenTerritories.insert(territory);
            else
                return false;
        }
    }


    return seenTerritories.size()==getTerritoryList().size();
}

bool Graph::validate() {
    bool connectedTerritories = isGraphConnected();
    bool connectedContinents = isContinentSubgraphConnected();
    bool oneToOneCorrespondence = isTerritoryContinentOneToOne();

    cout << boolalpha << "Territories: " << connectedTerritories << ", Continents: " << connectedContinents
         << ", One-to-One: " << oneToOneCorrespondence << endl;
    return
            connectedTerritories &&
            connectedContinents &&
            oneToOneCorrespondence;
}

Graph::Graph(const Graph &original) {
    territoryList = original.territoryList;
    continentList = original.continentList;
}

/**
 * Continent Class implementation
 */
Continent::Continent() : continentId(), continentName(),
                         territories(), bonus() {}

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

    if (isSameOwner()) {
        return getTerritories().at(0)->getOwner();
    }
    return "";
}

Continent::Continent(const Continent &original) {
    continentId = original.continentId;
    bonus = original.bonus;
    continentName = original.continentName;
    territories = original.territories;
}
void Continent::setTerritories(vector<Territory *> territories){
    this->territories = territories;
}