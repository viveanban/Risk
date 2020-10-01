//
// Created by tarek ait hamouda on 2020-09-23.
//

#include <iostream>
#include <set>
#include <vector>
#include "Map.h"

using namespace std;

/**
 * Territory Class implementation
 */
Territory::Territory() : territoryName(), territoryId(new int(0)), unitNbr(0), continentId(new int(0)), owner() {}

Territory::Territory(string territoryName, int territoryId, int unitNbr, int continentId, string owner, vector<Territory *> adjList) : territoryName(
        territoryName), territoryId(new int(territoryId)), unitNbr(unitNbr), continentId(new int(continentId)), owner(owner), adjList(adjList) {}

string &Territory::getTerritoryName() {
    return territoryName;
}

void Territory::setTerritoryName(const string &territoryName) {
    this->territoryName = territoryName;
}

int Territory::getTerritoryId() {
    return *this->territoryId;
}

void Territory::setTerritoryId(int territoryId) {
    *this->territoryId = territoryId;
}

int &Territory::getUnitNbr() {
    return unitNbr;
}

void Territory::setUnitNbr(int unitNbr) {
    this->unitNbr = unitNbr;
}

int Territory::getContinentId() {
    return *this->continentId;
}

void Territory::setContinentId(int continentId) {
    *this->continentId = continentId;
}

string &Territory::getOwner() {
    return owner;
}

void Territory::setOwner(const string &owner) {
    Territory::owner = owner;
}

void Territory::addLink(Territory *t) {
    adjList.push_back(t);
}

vector<Territory *> & Territory::getAdjList() {
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
    vector<Territory*> toVisit {getTerritoryList()[0]};

    for(Territory* territory: this-> getTerritoryList()){
        territories.insert(pair<Territory*, bool>(territory, false));
    }

    // now run dfs and visit only non visited territories

    while(!toVisit.empty()){

        //pop first element
        Territory currentTerritory = *toVisit[0];
        toVisit.erase(toVisit.begin());

        //add adjacent territory to toVisit list only if not visited in past

        // for(Territory* territory: currentTerritory.getAdjMap())
        // update the territories map when you visit a territory
        //check if there is any unvisited territory`

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
Continent::Continent() : continentId(), continentName(), territoriesInContinent(), bonus(0) {}

Continent::Continent(int continentId, string continentName, int bonus) : continentId(continentId), continentName(continentName), territoriesInContinent(),
                                                        bonus(bonus) {}

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

vector<Territory *> & Continent::getTerritoriesInContinent() {
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

string Continent::getOwner() {
    if (isSameOwner()) {
        return this->getTerritoriesInContinent()[0]->getOwner();
    }
}
