#include <iostream>
#include "Player.h"

int main() {

    std::cout << "Setting Up.." << std::endl;

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

    // Create Hand
    vector<Card *> cardsP1;
    cardsP1.push_back(new Card(Card::bomb));
    cardsP1.push_back(new Card(Card::blockade));
    cardsP1.push_back(new Card(Card::airlift));
    Hand *handP1 = new Hand(cardsP1);

    vector<Card *> cardsP2;
    cardsP2.push_back(new Card(Card::reinforcement));
    cardsP2.push_back(new Card(Card::diplomacy));
    Hand *handP2 = new Hand(cardsP2);

    std::cout << "Creating Player 1.." << std::endl;
    vector<Territory *> territoriesOwnedP1;
    territoriesOwnedP1.push_back(t1);
    territoriesOwnedP1.push_back(t2);

    Player* p1 = new Player();
    p1->setPlayerName("Player 1");
    p1->setTerritories(territoriesOwnedP1);
    p1->setHandOfCards(handP1);

    std::cout << "Creating Player 2.." << std::endl;
    vector<Territory *> territoriesOwnedP2;
    territoriesOwnedP2.push_back(t3);
    territoriesOwnedP2.push_back(t4);
    territoriesOwnedP2.push_back(t5);

    Player* p2 = new Player();
    p2->setPlayerName("Player 2");
    p2->setTerritories(territoriesOwnedP2);
    p2->setHandOfCards(handP2);

    std::cout << "Executing toDefend().." << std::endl;
    vector<Territory *> toDefendP1 = p1->toDefend();
    vector<Territory *> toDefendP2 = p2->toDefend();

    std::cout << "Executing toAttack().." << std::endl;
    vector<Territory *> toAttackP1 = p1->toAttack();
    vector<Territory *> toAttackP2 = p2->toAttack();

    std::cout << "Executing issueOrder().." << std::endl;
    p1->issueOrder();
    p2->issueOrder();

    std::cout << "Execution of Player Terminated" << std::endl;
}
