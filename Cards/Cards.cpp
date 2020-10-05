//
// Created by tarek ait hamouda on 2020-10-04.
//

#include "Cards.h"
#include <vector>

using namespace std;

/**
 * class Card {
private:
    cardType type;
public:
    void play();
};
**/

Card::Card(): type(){

}

Card::Card(cardType type): type(type) {

}

void Card::play() {
// to add
}

/**
 * DECK class
 */
Deck::Deck(): cards() {}

Deck::Deck(vector<Card *> cards) : cards(cards) {}

Card &Deck::draw() {
    int randomIndex = rand() % cards.size();
    Card &card = (*cards.at(randomIndex));
    cards.erase(cards.begin() + randomIndex);
    return card;
}

