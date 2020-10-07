#ifndef RISK_PLAYER_H
#define RISK_PLAYER_H

#include "../Map/Map.h"
#include <string>

using namespace std;

/**
* This class represents a Player in the Risk game.
*/
class Player {
private:
    string playerName;
    vector<Territory *> territories;
    //    OrdersList* orders;
    //    Hand* cards;
public:
     Player();
     ~Player();

     Player(const Player &original);

     Player& operator=(const Player& otherPlayer);

     friend std::ostream &operator<<(std::ostream &stream, Player player);

     string getPlayerName();

     void setPlayerName(string playerName);

     vector<Territory *> &getTerritories();

     void setTerritories(vector<Territory *> &territories);

     // TODO: create setters and getters for other two variables

     /**
     * This method returns a list of territories that need to be defended.
     *
     * @return Vector list of Territory pointers
     */
     vector<Territory *> toDefend();

    /**
    * This method returns a list of territories that need to be attacked.
    *
    * @return Vector list of Territory pointers
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
