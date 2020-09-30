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
Territory::Territory() : territoryName(), unitNbr(0), continent(), owner() {}

Territory::Territory(string territoryName, int unitNbr, Continent *continent, string owner) : territoryName(
        territoryName), unitNbr(unitNbr), continent(continent), owner(owner) {}

string &Territory::getTerritoryName() {
    return territoryName;
}

void Territory::setTerritoryName(const string &territoryName) {
    this->territoryName = territoryName;
}

Continent *Territory::getContinent() {
    return continent;
}

void Territory::setContinent(Continent *continent) {
    this->continent = continent;
}

int &Territory::getUnitNbr() {
    return unitNbr;
}

void Territory::setUnitNbr(int unitNbr) {
    this->unitNbr = unitNbr;
}

string &Territory::getOwner() {
    return owner;
}

void Territory::setOwner(const string &owner) {
    Territory::owner = owner;
}

/**
 * Node Class implementation
 */
Node::Node() : territory(), adjList(vector<Node *>()) {}

Node::Node(Territory territory) : territory(territory) {}

Node::Node(Territory territory, vector<Node*> adjList) : territory(territory), adjList(adjList) {}

void Node::addLink(Node *n) {
    adjList.push_back(n);
}

vector<Node *> Node::getAdjList() {
    return adjList;
}

void Node::setAdjList(vector<Node *> adjList) {
    this->adjList = adjList;
}

Territory Node::getTerritory() {
    return territory;
}

void Node::setTerritory(Territory territory) {
    this->territory = territory;
}

/**
 * Graph Class implementation
 */
Graph::Graph() : nodeList() {}

Graph::Graph(vector<Node *> nodeList) : nodeList(nodeList) {}

vector<Node *> Graph::getNodeList() {
    return nodeList;
}

void Graph::setNodeList(vector<Node *> nodeList) {
    this->nodeList = nodeList;
}

void Graph::addNode(Node *node) {
    nodeList.push_back(node);
}

bool Graph::isGraphConnected() {

    map<Node*, bool> territories;
    vector<Node*> toVisit {getNodeList()[0]};

    for(Node* node: this-> getNodeList()){
        territories.insert(pair<Node*, bool>(node, false));
    }

    // now run dfs and visit only non visited nodes

    while(!toVisit.empty()){

        //pop first element
        Node currentNode = *toVisit[0];
        toVisit.erase(toVisit.begin());

        //add adjacent node to toVisit list only if not visited in past

        // for(Territory* territory: currentNode.getAdjMap())
        // update the territories map when you visit a node
        //check if there is any unvisited node`

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

vector<Node *> Continent::getNodesInContinent() {
    return nodesInContinent;
}

void Continent::setNodesInContinent(vector<Node *> nodesInContinent) {
    this->nodesInContinent = nodesInContinent;
}

void Continent::addNodeInContinent(Node *n) {
    nodesInContinent.push_back(n);
}

bool Continent::isSameOwner() {
    set<Node *> setOfNodesInContinent;
    for (Node *node : this->getNodesInContinent()) {
        setOfNodesInContinent.insert(node);
    }
    return setOfNodesInContinent.size() == 1;
}

string Continent::getOwner() {
    if (isSameOwner()) {
        return this->getNodesInContinent()[0]->getTerritory().getOwner();
    }
}
