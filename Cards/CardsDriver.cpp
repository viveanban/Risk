#include "./Cards.h"
#include <iostream>

using namespace std;

int main() {
    srand((unsigned) time(nullptr));

    Deck *deck = new Deck(10);

    cout << "Deck size: " << deck->getCards().size() << endl;
    cout << "Picking 7 cards randomly and adding them to hand" << endl;

    cout << "=======================================" << endl;
    vector<Card *> cards;
    for (int i; i < 8; i++)
        cards.push_back(deck->draw());
    cout << "=======================================" << endl;
    Hand *hand = new Hand(cards);

    cout << "Deck size: " << deck->getCards().size() << endl;
    cout << "Cards currently in hand: " << hand->getCards().size() << endl;

    cout << "=========================================================" << endl;

    while (hand->getCards().size() != 0) {
        (*hand->getCards().at(0)).play();
        (*hand).removeCard(0);
        (*deck).addCard(cards.at(0));
    }

    cout << "=========================================================" << endl;
    cout << "Finished playing card in hand " << endl;

    cout << "Deck size: " << deck->getCards().size() << endl;
    cout << "Cards currently in hand: " << hand->getCards().size() << endl;
}