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

class Territory {
private:
    string territoryName;
    int unitNbr;
    string continentName;
public:
    Territory();
    Territory(string territoryName, int unitNbr, string continentName);

    string getTerritoryName();
    void setTerritoryName(string territoryName);

    string getContinentName();
    void setContinentName(string continentName);

    int getUnitNbr();
    void setUnitNbr(int unitNbr);
};

class Node {
private:
    Territory territory;
    map<Edge, Node> adjMap;
public:
    Node();
    Node(Territory territory);
    Node(Territory territory, map<Edge, Node> adjMap);

    void addLink(Node *n, Edge *e);

    map<Edge, Node> getAdjMap();
    void setAdjMap(map<Edge, Node> adjMap);

    Territory getTerritory();
    void setTerritory(Territory territory);
};

class Graph{
private:
    vector<Node> nodeList;
public:
    vector<Node> getNodeList();
    void setNodeList(vector<Node> nodeList);
};

class Continent {
public:
    Continent(const string &continentName);

private:
    string continentName;
    vector<Node> nodesInContinent;
    int bonus;
public:
    Continent();
    Continent(string continentName, int bonus);

    string getContinentName();
    void setContinentName(string continentName);

    int getBonus();
    void setBonus(int bonus);

    vector<Node> getNodesInContinent();
    void setNodesInContinent(vector<Node> nodesInContinent);

    void addNodeInContinent(Node n);

};