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
    int numberOfArmies;
    vector<Territory *> territories;
    OrdersList *orders;
    Hand *handOfCards;
    vector<Player*> playersNotToAttack;
public:
    static Player* neutralPlayer;

    const vector<Player *> &getPlayersNotToAttack() const;

    void setPlayersNotToAttack(const vector<Player *> &playersNotToAttack);

private:

    void issueBombOrder();

    void issueDeployOrder();

    void issueAdvanceOrder();

    void issueAirliftOrder();

    void issueBlockadeOrder();

    void issueReinforcementOrder();

    void issueNegotiateOrder();

    Card::CardType getRandomCard();

public:
    Player();

    Player(string playerName);

    ~Player();

    Player(const Player &original);

    Player &operator=(const Player &otherPlayer);

    friend std::ostream &operator<<(std::ostream &stream, Player &player);

    // Getters
    string getPlayerName();

    vector<Territory *> &getTerritories();

    Hand *getHandofCards();

    OrdersList *getOrders();

    int getNumberofArmies();

    // Setters
    void setPlayerName(string playerName);

    void setTerritories(vector<Territory *> &territories);

    void setHandOfCards(Hand *handOfCards);

    void setOrders(OrdersList *orders);

    void setNumberOfArmies(int numberOfArmies);

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
    * @return true when player wants to continue issuing order
    * @return false when player is done issuing order
    */
    bool issueOrder();

};

#endif //RISK_PLAYER_H
