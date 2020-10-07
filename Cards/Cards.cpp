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

Card::Card(CardType type) : type(type) {}

Card::Card(const Card &original) {
    type = original.type;
}

Card &Card::operator=(const Card &otherCard) {
    type = otherCard.type;
    return *this;
}

std::ostream &operator<<(std::ostream &stream, Card c) {
    return stream << "\tInformation on Card object:" << endl;
}

void Card::play() {
    cout << "Playing a card" << endl;
    switch (type) {
        case CardType::bomb:
            // return new order bomb
            break;
        case CardType::reinforcement:
            // return new order reinforcement
            break;
        case CardType::blockade:
            // return new order blockade
            break;
        case CardType::airlift:
            // return new order airlift
            break;
        case CardType::diplomacy:
            // return new order diplomacy
            break;
    }
}

/**
 * DECK class
 */
Deck::Deck(int size) {
    int counter = 0;
    srand((unsigned) time(0));
    while (counter < size) {
        switch (rand() % 5) {
            case 0:
                cards.push_back(new Card(Card::bomb));
                break;
            case 1:
                cards.push_back(new Card(Card::reinforcement));
                break;
            case 2:
                cards.push_back(new Card(Card::blockade));
                break;
            case 3:
                cards.push_back(new Card(Card::airlift));
                break;
            case 4:
                cards.push_back(new Card(Card::diplomacy));
                break;
        }
        counter++;
    }
}

Deck::Deck(const Deck &original) {
    cards = vector<Card *>(original.cards.size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*original.cards[i]);
}

Deck::~Deck() {
    for (auto p : cards)
        delete p;
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

Card *Deck::draw() {
    srand((unsigned) time(0));
    int randomIndex = rand() % cards.size();
    Card *card = cards.at(randomIndex);
    cards.erase(cards.begin() + randomIndex);
    return card;
}

void Deck::addCard(Card *card) {
    cout << "Adding card to deck" << endl;
    cards.push_back(card);
}

/**
 * Hand class
 */

Hand::Hand(vector<Card *> cards) : cards(cards) {}

Hand::Hand(const Hand &original) {
    cards = vector<Card *>(original.getCards().size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*original.getCards().at(i));
}

const vector<Card *> &Hand::getCards() const {
    return cards;
}

void Hand::setCards(const vector<Card *> &cards) {
    Hand::cards = cards;
}

int Hand::getCardNbr() const {
    return cardNbr;
}

void Hand::setCardNbr(int cardNbr) {
    Hand::cardNbr = cardNbr;
}

void Hand::removeCard(int index) {
    cout << "Removing card from hand" << endl;
    cards.erase(cards.begin() + index);
}