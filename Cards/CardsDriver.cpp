#include "./Cards.h"
#include <iostream>

using namespace std;

int main() {
    Deck *deck = new Deck(10);

    cout << "Deck size: "<< deck->getCards().size() << endl;
    cout << "Drawing some cards... "<< endl;

    Hand *hand = new Hand();
    for(int i; i < 8; i++){
        hand->addCard(deck->draw());
    }
    cout << "Deck size: "<< deck->getCards().size() << endl;
    cout << "Cards currently in hand: "<< hand->getCards().size() << endl;

    cout << "=========================================================" << endl;
    cout << "Playing cards in hand " << endl;

    while (hand->getCards().size()!=0) {
        (*hand->getCards().at(0)).play();
        (*deck).addCard(hand->getCards().at(0));
        (*hand).removeCard(0);

    }

    cout << "=========================================================" << endl;
    cout << "Finished playing card in hand " << endl;

    cout << "Deck size: "<< deck->getCards().size() << endl;
    cout << "Cards currently in hand: "<< hand->getCards().size() << endl;

    delete hand;
    hand = nullptr;

    delete deck;
    deck = nullptr;
}