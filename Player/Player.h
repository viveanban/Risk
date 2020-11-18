#ifndef RISK_PLAYER_H
#define RISK_PLAYER_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include <string>
#include <set>

using namespace std;

/**
* This class represents a Player in the Risk game.
*/
class Player {
private:
    string playerName;

    int numberOfArmies;

    vector<Territory *> territories;

    OrdersList *orders;

    Hand *handOfCards;

    set<Player *> playersNotToAttack;

    void sortTerritoryList(vector<Territory *> &territoryList);

    void playReinforcementCard();
    
    void issueDeployOrder();

    void issueAdvanceOrder();

    void issueOrderFromCard(Card *cardChosen);

public:
    static Player *neutralPlayer;

    explicit Player(string playerName);

    ~Player();

    Player(const Player &original);

    Player &operator=(const Player &otherPlayer);

    friend std::ostream &operator<<(std::ostream &stream, Player &player);

    void addTerritory(Territory *territory);

    void removeTerritory(Territory *territory);

    // Getters
    string getPlayerName();

    vector<Territory *> &getTerritories();

    Hand *getHandOfCards();

    OrdersList *getOrders();

    int getNumberofArmies();

    set<Player *> &getPlayersNotToAttack();

    // Setters
    void setNumberOfArmies(int numberOfArmies);

    /**
    * This method returns a list of territories that can be defended.
    *
    * @return List of all territories that can be defended
    */
    vector<Territory *> toDefend();

    /**
     * Returns the territories that can be defended from a source territory
     *
     * @param srcTerritory from which to defend
     * @return List of territories that can be defended from a srcTerritory
     */
    vector<Territory *> toDefend(Territory *srcTerritory);

    /**
    * This method returns a list of territories that can be attacked.
    *
    * @return List of enemy territories
    */
    vector<Territory *> toAttack();

    /**
     * This method returns a list of territories that can be attacked from a source territory.
     *
     * @param srcTerritory from which to attack
     * @return List of territories that can be attacked from a srcTerritory
     */
    vector<Territory *> toAttack(Territory *srcTerritory);

    /**
    * This method allows the Player to issue an order.
    *
    * @return true when player wants to continue issuing order
    * @return false when player is done issuing order
    */
    bool issueOrder();
};

#endif //RISK_PLAYER_H
