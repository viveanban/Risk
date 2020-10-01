#ifndef RISK_MAP_H
#define RISK_MAP_H

#include <vector>
#include <map>
#include <string>

using namespace std;

/**
 * This class represents a territory in the Risk game
 * A Territory represents any land that can be owned by a player.
 * Any territory must belong to a continent and have a name
 */
class Territory {
private:
    string territoryName;
    string owner; // until we have a Player object we will use string
    int continentId;
    int territoryId;
    int unitNbr;
    vector<Territory *> adjList;
public:
    Territory();

    Territory(const Territory &original);

    Territory& operator=(const Territory& otherTerritory);

    string getTerritoryName();

    void setTerritoryName(string territoryName);

    string getOwner();

    void setOwner(string owner);

    int getTerritoryId();

    void setTerritoryId(int territoryId);

    int getUnitNbr();

    void setUnitNbr(int unitNbr);

    int getContinentId();

    void setContinentId(int continentId);

    void addLink(Territory *n);

    vector<Territory *> &getAdjList();

    void setAdjList(vector<Territory *> &adjList);
};

/**
 * This class represents a Continent in the risk game
 * A continent contains a list of territories inside the continent as well as a bonus for players that own all
 * territories inside this continent
 */
class Continent {
private:
    int continentId;
    int bonus;
    string continentName;
    vector<Territory *> territories;

    bool isSameOwner();

public:
    Continent();

    Continent(const Continent &original);

    Continent& operator=(const Continent& otherContinent);

    int getContinentId();

    void setContinentId(int continentId);

    string getContinentName();

    void setContinentName(string continentName);

    int getBonus();

    void setBonus(int bonus);

    vector<Territory *> &getTerritories();
    void setTerritories(vector<Territory *> territories);

    string getOwner();
};

/**
 * This class represents a graph in the Risk game
 * A Graph is a list of connected Territories. In other words, it represents a list of countries
 * and continent that are interconnected with each other
 */
class Graph {
private:
    vector<Territory *> territoryList;

    vector<Continent *> continentList;

    bool isGraphConnected();

    bool isContinentSubgraphConnected();

    bool isTerritoryContinentOneToOne();

public:
    Graph();

    Graph(vector<Territory *> &territoryList, vector<Continent *> &continentList);

    Graph(const Graph &original);

    Graph& operator=(const Graph& otherGraph);

    vector<Territory *> &getTerritoryList();

    void setTerritoryList(vector<Territory *> &territoryList);

    vector<Continent *> &getContinentList();

    void setContinentList(vector<Continent *> &continentList);

    void addContinent(Continent *continent);

    void addTerritory(Territory *territory);

    bool validate();
};

#endif