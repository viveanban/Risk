//
// Created by tarek ait hamouda on 2020-09-23.
//

#include <iostream>
#include <set>
#include <vector>
#include "Map.h"

using namespace std;

Territory::Territory() : territoryName(), unitNbr(0), continentName() {}

Territory::Territory(string territoryName, int unitNbr, string continentName) : territoryName(territoryName),
                                                                                unitNbr(unitNbr),
                                                                                continentName(continentName) {}

string Territory::getTerritoryName() {
    return territoryName;
}

void Territory::setTerritoryName(string territoryName) {
    this->territoryName = territoryName;
}

string Territory::getContinentName() {
    return continentName;
}

void Territory::setContinentName(string continentName) {
    this->continentName = continentName;
}

int Territory::getUnitNbr() {
    return unitNbr;
}

void Territory::setUnitNbr(int unitNbr) {
    this->unitNbr = unitNbr;
}

Node::Node() : territory(), adjMap(map<Edge *, Node *>()) {}

Node::Node(Territory territory) : territory(territory) {}

Node::Node(Territory territory, map<Edge *, Node *> adjMap) : territory(territory), adjMap(adjMap) {}

void Node::addLink(Node *n, Edge *e) {
    adjMap.insert(pair<Edge *, Node *>(e, n));
}

map<Edge *, Node *> Node::getAdjMap() {
    return adjMap;
}

void Node::setAdjMap(map<Edge *, Node *> adjMap) {
    this->adjMap = adjMap;
}

Territory Node::getTerritory() {
    return territory;
}

void Node::setTerritory(Territory territory) {
    this->territory = territory;
}

Graph::Graph() : nodeList() {}

Graph::Graph(vector<Node> nodeList) : nodeList(nodeList) {}

vector<Node> Graph::getNodeList() {
    return nodeList;
}

void Graph::setNodeList(vector<Node> nodeList) {
    this->nodeList = nodeList;
}

Continent::Continent() : continentName(), nodesInContinent(), bonus(0) {}

Continent::Continent(string continentName, int bonus) : continentName(continentName), nodesInContinent(),
                                                        bonus(bonus) {}

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

vector<Node> Continent::getNodesInContinent() {
    return nodesInContinent;
}

void Continent::setNodesInContinent(vector<Node> nodesInContinent) {
    this->nodesInContinent = nodesInContinent;
}

void Continent::addNodeInContinent(Node n) {
    nodesInContinent.push_back(n);
}