//
// Created by tarek ait hamouda on 2020-10-04.
//

#include "Cards.h"
#include <vector>
#include <iostream>
#include <ctime>

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
    return stream << "\tInformation on Card object:" <<
                  "\tCard type: " << c.type << endl;
}

void Card::play() {
    switch (type) {
        case CardType::bomb:
            cout << "played bomb " << endl;
            break;
        case CardType::reinforcement:
            cout << "played reinforcement " << endl;
            break;
        case CardType::blockade:
            cout << "played blockade " << endl;
            break;
        case CardType::airlift:
            cout << "played airlift " << endl;
            break;
        case CardType::diplomacy:
            cout << "played diplomacy " << endl;
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
    for (auto p : cards) {
        delete p;
        p = NULL;
    }
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

const vector<Card *> &Deck::getCards() const {
    return cards;
}

void Deck::setCards(const vector<Card *> &cards) {
    Deck::cards = cards;
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

Hand::Hand() : cardNbr(), cards() {}

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