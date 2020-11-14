//#include <iostream>
//#include "Player.h"
//
//void printList(string message, vector<Territory *> list){
//    cout << message << endl;
//    for(Territory * territory: list) {
//        cout << territory <<  " => " << territory->getTerritoryName() << endl;
//    }
//}
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
//    t1->setUnitNbr(1);
//
//    Territory *t2 = new Territory();
//    t2->setTerritoryId(2);
//    t2->setTerritoryName("Territory 2");
//    t2->setUnitNbr(2);
//
//    Territory *t3 = new Territory();
//    t3->setTerritoryId(3);
//    t3->setTerritoryName("Territory 3");
//    t3->setUnitNbr(3);
//
//    Territory *t4 = new Territory();
//    t4->setTerritoryId(4);
//    t4->setTerritoryName("Territory 4");
//    t4->setUnitNbr(4);
//
//    Territory *t5 = new Territory();
//    t5->setTerritoryId(5);
//    t5->setTerritoryName("Territory 5");
//    t5->setUnitNbr(5);
//
//    t1->getAdjList().push_back(t3);
//    t1->getAdjList().push_back(t4);
//    t1->getAdjList().push_back(t2);
//
//    t2->getAdjList().push_back(t1);
//    t2->getAdjList().push_back(t3);
//
//    t3->getAdjList().push_back(t1);
//    t3->getAdjList().push_back(t2);
//    t3->getAdjList().push_back(t4);
//
//    t4->getAdjList().push_back(t1);
//    t4->getAdjList().push_back(t3);
//    t4->getAdjList().push_back(t5);
//
//    t5->getAdjList().push_back(t4);
//
//    vector<Territory *> territoriesOwnedP1;
//    territoriesOwnedP1.push_back(t1);
//    territoriesOwnedP1.push_back(t3);
//
//    vector<Territory *> territoriesOwnedP2;
//    territoriesOwnedP2.push_back(t4);
//    territoriesOwnedP2.push_back(t5);
//    territoriesOwnedP2.push_back(t2);
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
//    t1->setOwner(p1);
//    t3->setOwner(p1);
//    p1->setHandOfCards(handP1);
//    cout << *p1 << endl;
//
//    cout << "Creating Player 2..\n" << endl;
//    Player* p2 = new Player();
//    p2->setPlayerName("Player 2");
//    p2->setTerritories(territoriesOwnedP2);
//    t4->setOwner(p2);
//    t5->setOwner(p2);
//    t2->setOwner(p2);
//    p2->setHandOfCards(handP2);
//    cout << *p2 << endl;
//
//    cout << "--------Executing toDefend()--------\n" << endl;
//
//    printList("Territories to defend by Player 1", p1->toDefend());
//    printList("Territories to defend by Player 1 from t1", p1->toDefend(t1));
//    printList("Territories to defend by Player 1 from t3", p1->toDefend(t3));
//
//    cout << "---------------------------------------------------------------" << endl;
//
//    printList("Territories to defend by Player 2", p2->toDefend());
//    printList("Territories to defend by Player 2 from t2", p2->toDefend(t2));
//    printList("Territories to defend by Player 2 from t4", p2->toDefend(t4));
//    printList("Territories to defend by Player 2 from t5", p2->toDefend(t5));
//
//    cout << "--------Executing toAttack()--------\n" << endl;
//
//    printList("Territories to attack by Player 1",  p1->toAttack());
//    printList("Territories to attack by Player 1 from t1", p1->toAttack(t1));
//    printList("Territories to attack by Player 1 from t3", p1->toAttack(t3));
//
//    cout << "---------------------------------------------------------------" << endl;
//
//    printList("Territories to attack by Player 2",  p2->toAttack());
//    printList("Territories to attack by Player 2 from t2", p2->toAttack(t2));
//    printList("Territories to attack by Player 2 from t4", p2->toAttack(t4));
//    printList("Territories to attack by Player 2 from t5", p2->toAttack(t5));
//
//    cout << "--------Executing issueOrder()--------" << endl;
//    p1->issueOrder();
//    p2->issueOrder();
//
//    cout << "--------Execution of Player Terminated--------" << endl;
//}
//
//
