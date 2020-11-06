#ifndef RISK_PLAYER_H
#define RISK_PLAYER_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include <string>

using namespace std;

/**
* This class represents a Player in the Risk game.
*/
class Player {
private:
    string playerName;
    vector<Territory *> territories;
    OrdersList* orders;
    Hand* handOfCards;
public:
     Player();

     ~Player();

     Player(const Player &original);

     Player& operator=(const Player& otherPlayer);

     friend std::ostream &operator<<(std::ostream &stream, Player &player);

     // Getters
     string getPlayerName();

     vector<Territory *> &getTerritories();

     Hand* getHandofCards();

     OrdersList* getOrders();

     // Setters
     void setPlayerName(string playerName);

     void setTerritories(vector<Territory *> &territories);

     void setHandOfCards(Hand *handOfCards);

     void setOrders(OrdersList* orders);

     void addTerritory(Territory* territory);
     /**
     * This method returns a list of territories that need to be defended.
     *
     * @return Vector list of Territory pointers
     */
     vector<Territory *> toDefend();

    /**
    * This method returns a list of territories that need to be attacked.
    *
    * @return Vector list of Territory pinters
    */
    vector<Territory *> toAttack();

    /**
    * This method allows the Player to issue an order.
    *
    * @return void
    */
    void issueOrder();
};

#endif //RISK_PLAYER_H
