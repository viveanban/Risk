//
// Created by tarek ait hamouda on 2020-09-23.
//

#include <iostream>
#include <set>
#include <vector>
#include "Map.h"

using namespace std;

Territory::Territory() : territoryName(), unitNbr(0), continentName(){}

Territory::Territory(string territoryName, int unitNbr, string continentName) : territoryName(territoryName), unitNbr(unitNbr), continentName(continentName){}

string Territory::getTerritoryName(){
    return territoryName;
}
void Territory::setTerritoryName(string territoryName){
    this->territoryName = territoryName;
}

string Territory::getContinentName(){
    return continentName;
}
void Territory::setContinentName(string continentName){
    this -> continentName = continentName;
}

int Territory::getUnitNbr(){
    return unitNbr;
}
void Territory::setUnitNbr(int unitNbr){
    this->unitNbr = unitNbr;
}

Node::Node(): territory(),  adjMap(map<Edge*, Node*>()){}

Node::Node(Territory territory): territory(territory){}

Node::Node(Territory territory, map<Edge, Node> adjMap): territory(territory), adjMap(adjMap){}

void Node:: addLink(Node *n, Edge *e){
    adjMap.insert(pair<Edge*, Node*>(e,n));
}