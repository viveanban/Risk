#ifndef RISK_MAP_H
#define RISK_MAP_H

#include <vector>
#include <map>
#include <string>

using namespace std;

class Player;

/**
 * This class represents a territory in the Risk game
 * A Territory represents any land that can be owned by a player.
 * Any territory must belong to a continent and have a name
 */
class Territory {
private:
    string territoryName;
    Player *owner;
    int continentId;
    int territoryId;
    int unitNbr;
    vector<int> adjList;
public:
    Territory();

    Territory(const Territory &original);

    // Copy constructor called from the Copy Constructor of Player
    // Create a new Territory based on an existing Territory but no new Player is created.
    Territory(const Territory &original, Player *player);

    Territory &operator=(const Territory &otherTerritory);

    friend std::ostream &operator<<(std::ostream &stream, Territory &c);

    // Getters
    string getTerritoryName();

    Player *getOwner();

    int getTerritoryId();

    int getUnitNbr();

    vector<int> &getAdjList();

    int getContinentId();

    // Setters
    void setTerritoryName(string territoryName);

    void setOwner(Player *owner);

    void setTerritoryId(int territoryId);

    void setUnitNbr(int unitNbr);

    void setContinentId(int continentId);

    void setAdjList(vector<int> &adjList);

    // Adds an edge between two territories
    void addLink(int id);
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

public:
    Continent();

    ~Continent();

    Continent(const Continent &original);

    Continent &operator=(const Continent &otherContinent);

    // output insertion stream operator override
    friend std::ostream &operator<<(std::ostream &stream, Continent &c);

    bool isSameOwner();

    // Getters
    int getContinentId();

    string getContinentName();

    int getBonus();

    Player* getOwner();

    vector<Territory *> &getTerritories();

    // Setters
    void setContinentId(int continentId);

    void setContinentName(string continentName);

    void setBonus(int bonus);

    void setTerritories(vector<Territory *> territories);
};

/**
 * This class represents a graph in the Risk game
 * A Map is a list of connected Territories. In other words, it represents a list of countries
 * and continent that are interconnected with each other
 */
class Map {
private:
    vector<Territory *> territoryList;

    vector<Continent *> continentList;

    // Used to validate if the graph is fully connected
    bool isMapConnected();

    // Used to validate if the continents are connected
    bool isContinentSubgraphConnected();

    // Used to validate if each territory exists in exactly one continent.
    bool isTerritoryContinentUnique();

public:
    Map();

    ~Map();

    Map(vector<Territory *> &territoryList, vector<Continent *> &continentList);

    Map(const Map &original);

    Map &operator=(const Map &otherGraph);

    friend std::ostream &operator<<(std::ostream &stream, Map &c);

    // Getters
    vector<Territory *> &getTerritoryList();

    vector<Continent *> &getContinentList();

    Territory *getTerritoryById(int id);

    // Setters
    void setTerritoryList(vector<Territory *> &territoryList);

    void setContinentList(vector<Continent *> &continentList);

    // Add continents individually
    void addContinent(Continent *continent);

    // Add territories individually
    void addTerritory(Territory *territory);

    // Used to validate the graph using the 3 defined conditions for connectivity.
    bool validate();
};

#endif