#include <iostream>
#include "Player.h"

int main() {

    // TODO: show that the Player can own hand of cards?

    // 1. Create players

    // Create Territories and their Adjacent Territories
    Territory *t1 = new Territory();
    t1->setTerritoryName("Territory 1");
    Territory *t2 = new Territory();
    t2->setTerritoryName("Territory 2");
    Territory *t3 = new Territory();
    t3->setTerritoryName("Territory 3");
    Territory *t4 = new Territory();
    t4->setTerritoryName("Territory 4");
    Territory *t5 = new Territory();
    t5->setTerritoryName("Territory 5");
    t1->getAdjList().push_back(t3);
    t1->getAdjList().push_back(t4);
    t1->getAdjList().push_back(t2);

    t2->getAdjList().push_back(t1);
    t2->getAdjList().push_back(t3);

    t3->getAdjList().push_back(t1);
    t3->getAdjList().push_back(t2);
    t3->getAdjList().push_back(t4);

    t4->getAdjList().push_back(t1);
    t4->getAdjList().push_back(t3);
    t4->getAdjList().push_back(t5);

    t5->getAdjList().push_back(t4);

    // Player 1
    vector<Territory *> territoriesOwnedP1;
    territoriesOwnedP1.push_back(t1);
    territoriesOwnedP1.push_back(t2);

    Player* p1 = new Player();
    p1->setTerritories(territoriesOwnedP1);
    p1->setPlayerName("Player 1");

    // Player 2
    vector<Territory *> territoriesOwnedP2;
    territoriesOwnedP2.push_back(t3);
    territoriesOwnedP2.push_back(t4);
    territoriesOwnedP2.push_back(t5);

    Player* p2 = new Player();
    p2->setTerritories(territoriesOwnedP2);
    p2->setPlayerName("Player 2");

    // 2. Demonstrate toDefend()
    vector<Territory *> toDefendP1 = p1->toDefend();
    vector<Territory *> toDefendP2 = p2->toDefend();

    // 3. Demonstrate toAttack()
    vector<Territory *> toAttackP1 = p1->toAttack();
    vector<Territory *> toAttackP2 = p2->toAttack();

    std::cout << "Execution of Player Terminated" << std::endl;

    // 4. Demonstrate issueOrder()
//    p1->issueOrder();
//    p2->issueOrder();

}
