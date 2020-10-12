//#include <iostream>
//#include "Player.h"
//
//int main() {
//
//    cout << "--------Setting Up--------" << endl;
//
//    // Create Territories and their Adjacent Territories
//    cout << "Creating Dummy Map.." << endl;
//    Territory *t1 = new Territory();
//    t1->setTerritoryId(1);
//    t1->setTerritoryName("Territory 1");
//    Territory *t2 = new Territory();
//    t2->setTerritoryId(2);
//    t2->setTerritoryName("Territory 2");
//    Territory *t3 = new Territory();
//    t3->setTerritoryId(3);
//    t3->setTerritoryName("Territory 3");
//    Territory *t4 = new Territory();
//    t4->setTerritoryId(4);
//    t4->setTerritoryName("Territory 4");
//    Territory *t5 = new Territory();
//    t5->setTerritoryId(5);
//    t5->setTerritoryName("Territory 5");
//
//    t1->getAdjList().push_back(t3->getTerritoryId());
//    t1->getAdjList().push_back(t4->getTerritoryId());
//    t1->getAdjList().push_back(t2->getTerritoryId());
//
//    t2->getAdjList().push_back(t1->getTerritoryId());
//    t2->getAdjList().push_back(t3->getTerritoryId());
//
//    t3->getAdjList().push_back(t1->getTerritoryId());
//    t3->getAdjList().push_back(t2->getTerritoryId());
//    t3->getAdjList().push_back(t4->getTerritoryId());
//
//    t4->getAdjList().push_back(t1->getTerritoryId());
//    t4->getAdjList().push_back(t3->getTerritoryId());
//    t4->getAdjList().push_back(t5->getTerritoryId());
//
//    t5->getAdjList().push_back(t4->getTerritoryId());
//
//    vector<Territory *> territoriesOwnedP1;
//    territoriesOwnedP1.push_back(t1);
//    territoriesOwnedP1.push_back(t2);
//
//    vector<Territory *> territoriesOwnedP2;
//    territoriesOwnedP2.push_back(t3);
//    territoriesOwnedP2.push_back(t4);
//    territoriesOwnedP2.push_back(t5);
//
//    // Create Hand
//    cout << "Creating Hand.." << endl;
//    Deck *deck = new Deck(10);
//    vector<Card *> cardsP1;
//    Hand *handP1 = new Hand(cardsP1);
//    handP1->addCard(deck->draw());
//    handP1->addCard(deck->draw());
//    handP1->addCard(deck->draw());
//
//    vector<Card *> cardsP2;
//    Hand *handP2 = new Hand(cardsP2);
//    handP2->addCard(deck->draw());
//    handP2->addCard(deck->draw());
//    handP2->addCard(deck->draw());
//
//    cout << "\nCreating Player 1..\n" << endl;
//    Player* p1 = new Player();
//    p1->setPlayerName("Player 1");
//    p1->setTerritories(territoriesOwnedP1);
//    p1->setHandOfCards(handP1);
//    cout << *p1 << endl;
//
//    cout << "Creating Player 2..\n" << endl;
//    Player* p2 = new Player();
//    p2->setPlayerName("Player 2");
//    p2->setTerritories(territoriesOwnedP2);
//    p2->setHandOfCards(handP2);
//    cout << *p2 << endl;
//
//    cout << "--------Executing toDefend()--------\n" << endl;
//
//    vector<int> toDefendP1 = p1->toDefend();
//    cout << "Territories to defend by Player 1\n" << endl;
//    for(auto territory: toDefendP1) {
//        cout << territory << endl;
//    }
//    vector<int> toDefendP2 = p2->toDefend();
//    cout << "Territories to defend by Player 2\n" << endl;
//    for(auto territory: toDefendP2) {
//        cout << territory << endl;
//    }
//
//    cout << "--------Executing toAttack()--------\n" << endl;
//
//    vector<int> toAttackP1 = p1->toAttack();
//    cout << "Territories to attack by Player 1\n" << endl;
//    for(auto territory: toAttackP1) {
//        cout << territory << endl;
//    }
//
//    vector<int> toAttackP2 = p2->toAttack();
//    cout << "Territories to attack by Player 2\n" << endl;
//    for(auto territory: toAttackP2) {
//        cout << territory << endl;
//    }
//
//    cout << "--------Executing issueOrder()--------" << endl;
//    p1->issueOrder();
//    p2->issueOrder();
//
//    cout << "--------Execution of Player Terminated--------" << endl;
//}
