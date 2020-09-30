#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include "Map.h"

using namespace std;

/**
 * Territory Class implementation
 */
Territory::Territory() : territoryName(), unitNbr(0), continent(), owner() {}

Territory::Territory(string* territoryName, int* unitNbr, Continent* continent, string* owner, vector<Territory *> adjList) : territoryName(
        territoryName), unitNbr(unitNbr), continent(continent), owner(owner), adjList(adjList) {}

string* Territory::getTerritoryName() {
    return territoryName;
}

void Territory::setTerritoryName(string* territoryName) {
    this->territoryName = territoryName;
}

Continent *Territory::getContinent() {
    return continent;
}

void Territory::setContinent(Continent* continent) {
    this->continent = continent;
}

int* Territory::getUnitNbr() {
    return unitNbr;
}

void Territory::setUnitNbr(int* unitNbr) {
    this->unitNbr = unitNbr;
}

string* Territory::getOwner() {
    return owner;
}

void Territory::setOwner(string* owner) {
    Territory::owner = owner;
}

void Territory::addLink(Territory* t) {
    adjList.push_back(t);
}

vector<Territory *> Territory::getAdjList() {
    return adjList;
}

void Territory::setAdjList(vector<Territory *> adjList) {
    this->adjList = adjList;
}

/**
 * Graph Class implementation
 */
Graph::Graph() : territoryList() {}

Graph::Graph(vector<Territory *> territoryList) : territoryList(territoryList) {}

vector<Territory *> Graph::getTerritoryList() {
    return territoryList;
}

void Graph::setTerritoryList(vector<Territory *> territoryList) {
    this->territoryList = territoryList;
}

void Graph::addTerritory(Territory *territory) {
    territoryList.push_back(territory);
}

bool Graph::isGraphConnected() {

    map<Territory*, bool> territories;
    stack<Territory*> toVisitStack;
    toVisitStack.push(getTerritoryList()[0]);

    for(Territory* territory: this-> getTerritoryList()){
        territories.insert(pair<Territory*, bool>(territory, false));
    }

    while(!toVisitStack.empty()){

        Territory* currentTerritory = toVisitStack.top();
        toVisitStack.pop();

        for(Territory* territory : currentTerritory->getAdjList()){
            if(territories[territory] == false){
                toVisitStack.push(territory);
            }
        }

        for(Territory* territory: getTerritoryList()){
            if(territories[territory] == false)
                return false;
        }
        return true;
    }

}

bool Graph::isContinentSubgraphConnected() {

}

bool Graph::isCountryContinentOneToOne() {

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
Continent::Continent() : continentName(), territoriesInContinent(), bonus(0) {}

Continent::Continent(string* continentName, int* bonus) : continentName(continentName), territoriesInContinent(),
                                                        bonus(bonus) {}

string* Continent::getContinentName() {
    return continentName;
}

void Continent::setContinentName(string* continentName) {
    this->continentName = continentName;
}


int* Continent::getBonus() {
    return bonus;
}

void Continent::setBonus(int* bonus) {
    this->bonus = bonus;
}

vector<Territory *> Continent::getTerritoriesInContinent() {
    return territoriesInContinent;
}

void Continent::setTerritoriesInContinent(vector<Territory *> territoriesInContinent) {
    this->territoriesInContinent = territoriesInContinent;
}

void Continent::addTerritoryInContinent(Territory *n) {
    territoriesInContinent.push_back(n);
}

bool Continent::isSameOwner() {
    set<Territory *> setOfTerritoriesInContinent;
    for (Territory *territory : this->getTerritoriesInContinent()) {
        setOfTerritoriesInContinent.insert(territory);
    }
    return setOfTerritoriesInContinent.size() == 1;
}

string* Continent::getOwner() {

    if (isSameOwner()) {
        return this->getTerritoriesInContinent()[0]->getOwner();
    }
}
