#ifndef RISK_MAP_H
#define RISK_MAP_H

#include <vector>
#include <map>
#include <string>

using namespace std;

//for circular dependency
class Continent;

/**
 * This class represents an edge in the graph in the Risk game
 * An edge represents a link/border between two Nodes
 */
class Edge {
private:

public:

};

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
public:
    Territory();

    Territory(string territoryName, int unitNbr, Continent *continent, string owner);

    string &getTerritoryName();

    void setTerritoryName(const string &territoryName);

    Continent* getContinent();

    void setContinent(Continent *continent);

    int &getUnitNbr();

    void setUnitNbr(int unitNbr);

    string &getOwner();

    void setOwner(const string &owner);
};

/**
 * This class represents a node in the graph
 * A Node contains a territory as well as a map of other adjacent node with their respective edges
 */
class Node {
private:
    Territory territory;
    map<Edge*, Node*> adjMap;
public:
    Node();
    Node(Territory territory);
    Node(Territory territory, map<Edge*, Node*> adjMap);

    void addLink(Node *n, Edge *e);

    map<Edge*, Node*> getAdjMap();
    void setAdjMap(map<Edge*, Node*> adjMap);

    Territory getTerritory();
    void setTerritory(Territory territory);
};

/**
 * This class represents a graph in the Risk game
 * A Graph is a list of connected Node. In other words, it represents a list of countries
 * and continent that are interconnected with each other
 */
class Graph{
private:
    vector<Node*> nodeList;
    bool isGraphConnected();
    bool isContinentSubgraphConnected();
    bool isCountryContinentOneToOne();
public:
    Graph();
    Graph(vector<Node*> nodeList);

    vector<Node*> getNodeList();
    void setNodeList(vector<Node*> nodeList);

    void addNode(Node* node);

    bool validate();
};

/**
 * This class represents a Continent in the risk game
 * A continent contains a list of nodes inside the continent as well as a bonus for players that own all
 * territories inside this continent
 */
class Continent {
private:
    string continentName;
    vector<Node*> nodesInContinent;
    int bonus;
    bool isSameOwner();
public:
    Continent();
    Continent(string continentName, int bonus);

    string getContinentName();
    void setContinentName(string continentName);

    int getBonus();
    void setBonus(int bonus);

    vector<Node*> getNodesInContinent();
    void setNodesInContinent(vector<Node*> nodesInContinent);

    void addNodeInContinent(Node* n);
    string getOwner();


};

#endif