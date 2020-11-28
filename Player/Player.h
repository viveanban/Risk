#ifndef RISK_PLAYER_H
#define RISK_PLAYER_H

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include <string>
#include <set>

using namespace std;

class PlayerStrategy;

/**
* This class represents a Player in the Risk game.
*/
class Player {
private:
    string playerName;

    int numberOfArmiesInReinforcementPool;

    vector<Territory *> territories;

    OrdersList *orders;

    Hand *handOfCards;

    set<Player *> playersNotToAttack;

    PlayerStrategy* strategy;

    bool isHumanPlayer;

public:
    static Player *neutralPlayer;

    explicit Player(string playerName);

    Player(string playerName, PlayerStrategy* strategy);

    ~Player();

    Player(const Player &original);

    Player &operator=(const Player &otherPlayer);

    friend std::ostream &operator<<(std::ostream &stream, Player &player);

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

    /**
     * Sort all territories from the list from the one with least unitNbr to the one with highest unitNbr
     * @param territoryList: represents the territoryList to sort
     */
    void sortTerritoryList(vector<Territory *> &territoryList);

    /**
     * Randomly decide if the player plays his reinforcement card, if yes then add +5 unit armies to the player.
     */
    void playReinforcementCard();

    /**
     * Issue a deploy order and potentially a reinforcement card.
     */
    void issueDeployOrder();

    /**
     * Issue an advance order, randomly decides whether to attack or defend and select a random territory to do so.
     */
    void issueAdvanceOrder();

    /**
     * Issue any non-deploy and non-advance order triggered by a card.
     * @param cardChosen : card to play
     */
    void issueOrderFromCard(Card *cardChosen);

    /**
     * Add a territory to the list of territory owned by the player
     * @param territory : territory to add to the list of owned territory by the user.
     */
    void addTerritory(Territory *territory);

    /**
     * Remove a territory from the list of territory owned by the player
     * @param territory : territory to remove from the list of owned territory by the user.
     */
    void removeTerritory(Territory *territory);

    // Getters
    string getPlayerName();

    vector<Territory *> &getTerritories();

    Hand *getHandOfCards();

    OrdersList *getOrders();

    int getNumberofArmiesInReinforcementPool();

    set<Player *> &getPlayersNotToAttack();

    // Setters
    void setNumberOfArmiesInReinforcementPool(int numberOfArmies);

    void setStrategy(PlayerStrategy *playerStrategy);

    PlayerStrategy *getStrategy() const;

    bool getIsHumanPlayer() const;

    static bool getBooleanInput(string printStatement);

    static int getIntegerInput(string printStatement);
};

#endif //RISK_PLAYER_H
