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
    return territoryName;
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
    return unitNbr;
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
    return owner;
}

void Territory::setOwner(string owner) {
    this->owner = owner;
}

void Territory::addLink(Territory *t) {
    adjList.push_back(t);
}

vector<Territory *> & Territory::getAdjList() {
    return adjList;
}

/**
 * Graph Class implementation
 */
Graph::Graph() : territoryList() {}

Graph::Graph(vector<Territory *> territoryList, vector<Continent *> continentList) : territoryList(territoryList), continentList(continentList) {}

vector<Territory *> & Graph::getTerritoryList() {
    return territoryList;
}

void Graph::setTerritoryList(vector<Territory *> territoryList) {
    this->territoryList = territoryList;
}

void Graph::addTerritory(Territory *territory) {
    territoryList.push_back(territory);
}

vector<Continent *> & Graph::getContinentList(){
    return continentList;
}

void Graph::setContinentList(vector<Continent *> continentList){
    this->continentList = continentList;
}

void Graph::addContinent(Continent *continent){
    continentList.push_back(continent);
}

bool Graph::isGraphConnected() {

    map<Territory *, bool> territories;
    stack<Territory *> toVisitStack;

    //set all territories to unvisited
    for (Territory *territory: getTerritoryList()) {
        territories.insert(pair<Territory *, bool>(territory, false));
    }

    toVisitStack.push(getTerritoryList().at(0));

    while (!toVisitStack.empty()) {

        //pop the top territories to visit it
        Territory *currentTerritory = toVisitStack.top();
        toVisitStack.pop();

        //add all unvisited territories to the stack to visit them later
        for (Territory *territory : (*currentTerritory).getAdjList()) {
            if (territories[territory] == false)
                toVisitStack.push(territory);
        }
    }

    //once we don't have anymore territories to visit in the stack,
    // we need to verify if we visited all territories
    for (Territory *territory: getTerritoryList()) {
        if (territories[territory] == false)
            return false;
    }
    return true;
}

bool Graph::isContinentSubgraphConnected() {
    return true;

}

bool Graph::isTerritoryContinentOneToOne() {
    return true;
}

bool Graph::validate() {
    return
            isGraphConnected() &&
            isContinentSubgraphConnected() &&
            isTerritoryContinentOneToOne();
}

/**
 * Continent Class implementation
 */
Continent::Continent() : continentId(), continentName(),
                         territories(), bonus() {}

int Continent::getContinentId() {
    return continentId;
}

void Continent::setContinentId(int continentId) {
    this->continentId = continentId;
}

string Continent::getContinentName() {
    return continentName;
}

void Continent::setContinentName(string continentName) {
    this->continentName = continentName;
}

int Continent::getBonus() {
    return bonus;
}

void Continent::setBonus(int bonus) {
    this->bonus = bonus;
}

vector<Territory *> & Continent::getTerritories() {
    return territories;
}

void Continent::setTerritoriesInContinent(vector<Territory *> territories) {
    this->territories = territories;
}

void Continent::addTerritoryInContinent(Territory *n) {
    territories.push_back(n);
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
}
