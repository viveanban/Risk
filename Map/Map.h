#ifndef RISK_MAP_H
#define RISK_MAP_H

#include <vector>
#include <map>
#include <string>

using namespace std;

//for circular dependency
class Continent;

/**
 * This class represents a territory in the Risk game
 * A Territory represents any land that can be owned by a player.
 * Any territory must belong to a continent and have a name
 */
class Territory {
private:
    string territoryName;
    Continent *continent;
    string owner; // until we have a Player object we will use string
    int unitNbr;
    vector<Territory *> adjList;
public:
    Territory();

    Territory(string territoryName, int unitNbr, Continent *continent, string owner, vector<Territory *> adjList);

    string &getTerritoryName();

    void setTerritoryName(const string &territoryName);

    Continent *getContinent();

    void setContinent(Continent *continent);

    int &getUnitNbr();

    void setUnitNbr(int unitNbr);

    string &getOwner();

    void setOwner(const string &owner);

    void addLink(Territory *n);

    vector<Territory *> getAdjList();

    void setAdjList(vector<Territory *> adjList);
};

/**
 * This class represents a graph in the Risk game
 * A Graph is a list of connected Territories. In other words, it represents a list of countries
 * and continent that are interconnected with each other
 */
class Graph {
private:
    vector<Territory *> territoryList;

    bool isGraphConnected();

    bool isContinentSubgraphConnected();

    bool isCountryContinentOneToOne();

public:
    Graph();

    Graph(vector<Territory *> territoryList);

    vector<Territory *> getTerritoryList();

    void setTerritoryList(vector<Territory *> territoryList);

    void addTerritory(Territory *territory);

    bool validate();
};

/**
 * This class represents a Continent in the risk game
 * A continent contains a list of territories inside the continent as well as a bonus for players that own all
 * territories inside this continent
 */
class Continent {
private:
    string continentName;
    vector<Territory *> territoriesInContinent;
    int bonus;

    bool isSameOwner();

public:
    Continent();

    Continent(string continentName, int bonus);

    string getContinentName();

    void setContinentName(string continentName);

    int getBonus();

    void setBonus(int bonus);

    vector<Territory *> getTerritoriesInContinent();

    void setTerritoriesInContinent(vector<Territory *> territoriesInContinent);

    void addTerritoryInContinent(Territory *n);

    string getOwner();


};

#endif