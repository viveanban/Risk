//
// Created by tarek ait hamouda on 2020-09-23.
//

#ifndef RISK_MAP_H
#define RISK_MAP_H

#endif //RISK_MAP_H

#include <vector>
#include <map>
#include <string>

using namespace std;

class Edge {
private:

public:

};

class Continent;

/**
 * This class represents a territory in Risk
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

class Graph{
private:
    vector<Node*> nodeList;
public:
    Graph();
    Graph(vector<Node*> nodeList);

    vector<Node*> getNodeList();
    void setNodeList(vector<Node*> nodeList);

    void addNode(Node* node);

    bool isGraphValid();
};

class Continent {
private:
    string continentName;
    vector<Node*> nodesInContinent;
    int bonus;
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

};