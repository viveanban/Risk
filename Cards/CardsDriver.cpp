//#include "./Cards.h"
//#include <iostream>
//
//using namespace std;
//
//int main() {
//    Deck *deck = new Deck(10);
//
//    cout << "Deck size: "<< deck->getCards().size() << endl;
//    cout << "Drawing some cards... "<< endl;
//
//    vector<Card *> cards;
//    cards.push_back(deck->draw());
//    cards.push_back(deck->draw());
//    cards.push_back(deck->draw());
//    cards.push_back(deck->draw());
//    cards.push_back(deck->draw());
//    cards.push_back(deck->draw());
//    cards.push_back(deck->draw());
//
//    Hand *hand = new Hand(cards);
//
//    cout << "Deck size: "<< deck->getCards().size() << endl;
//    cout << "Cards currently in hand: "<< hand->getCards().size() << endl;
//
//    cout << "=========================================================" << endl;
//    cout << "Playing cards in hand " << endl;
//
//    while (hand->getCards().size()!=0) {
//        (*hand->getCards().at(0)).play();
//        (*hand).removeCard(0);
//        (*deck).addCard(cards.at(0));
//    }
//
//    cout << "=========================================================" << endl;
//    cout << "Finished playing card in hand " << endl;
//
//    cout << "Deck size: "<< deck->getCards().size() << endl;
//    cout << "Cards currently in hand: "<< hand->getCards().size() << endl;
//}
//
