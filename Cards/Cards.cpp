//
// Created by tarek ait hamouda on 2020-10-04.
//

#include "Cards.h"
#include <vector>
#include <iostream>

using namespace std;

/**
 * Card class
**/

Card::Card() : type() {}

Card::Card(CardType type) : type(type) {}

Card::Card(const Card &original) {
    type = original.type;
}

Card &Card::operator=(const Card &otherCard) {
    type = otherCard.type;
    return *this;
}

std::ostream &operator<<(std::ostream &stream, Card c) {
    return stream << "\tInformation on Card object:" << endl <<
                  "\tCard type : " << c.getType() << endl;
}

void Card::setType(CardType type) {
    this->type = type;
}

CardType Card::getType() {
    return this->type;
}

void Card::play() {
// to add
}

/**
 * DECK class
 */
Deck::Deck() : cards() {}

Deck::Deck(vector<Card *> cards) : cards(cards) {}

Deck::Deck(const Deck &original) {
    cards = vector<Card *>(original.cards.size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*original.cards[i]);
}

Deck &Deck::operator=(const Deck &otherDeck) {
    cards = vector<Card *>(otherDeck.cards.size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*otherDeck.cards[i]);
    return *this;
}

std::ostream &operator<<(std::ostream &stream, Deck c) {
    return stream << "\tInformation on Deck object:" << endl <<
                  "\tNumber of cards in deck: " << c.getCards().size() << endl;
}

void Deck::setCards(vector<Card *> cards) {
    this->cards = cards;
}

vector<Card *> Deck::getCards() {
    return this->cards;
}

Card &Deck::draw() {
    int randomIndex = rand() % cards.size();
    Card &card = (*cards.at(randomIndex));
    cards.erase(cards.begin() + randomIndex);
    return card;
}

