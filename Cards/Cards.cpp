#include "Cards.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>

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

ostream &operator<<(ostream &stream, const Card &c) {
    return stream << "Information on Card object: " << endl
                  << "Card type: " << c.type << endl;
}

Order *Card::play() {
    Order *order;
    switch (type) {
        case CardType::bomb:
            order = new BombOrder();
            cout << "played bomb " << endl;
            break;
        case CardType::reinforcement:
            order = nullptr;
            cout << "played reinforcement " << endl;
            break;
        case CardType::blockade:
            order = new BlockadeOrder();
            cout << "played blockade " << endl;
            break;
        case CardType::airlift:
            order = new AirliftOrder();
            cout << "played airlift " << endl;
            break;
        case CardType::diplomacy:
            order = new NegotiateOrder();
            cout << "played diplomacy " << endl;
            break;
    }
    return order;
}

Card::CardType Card::getType() const {
    return type;
}

void Card::setType(Card::CardType type) {
    Card::type = type;
}

/**
 * Deck class
 */
Deck::Deck(int size) {
    int counter = 0;
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
        p = nullptr;
    }
}

Deck &Deck::operator=(const Deck &otherDeck) {
    cards = vector<Card *>(otherDeck.cards.size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*otherDeck.cards[i]);
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const Deck &d) {
    return stream << "Information on Deck object:" << endl
                  << "Number of cards in deck: " << d.getCards().size() << endl;
}

const vector<Card *> &Deck::getCards() const {
    return cards;
}

void Deck::setCards(const vector<Card *> &cards) {
    Deck::cards = cards;
}

Card *Deck::draw() {
    int randomIndex = rand() % cards.size();
    Card *card = cards.at(randomIndex);
    cout << *card;
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
Hand::Hand() : cards() {}

Hand::Hand(vector<Card *> cards) : cards(cards) {}

Hand::Hand(const Hand &original) {
    cards = vector<Card *>(original.getCards().size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*original.getCards().at(i));
}

Hand &Hand::operator=(const Hand &otherHand) {
    cards = vector<Card *>(otherHand.getCards().size());
    for (int i = 0; i < cards.size(); i++)
        cards[i] = new Card(*otherHand.getCards().at(i));
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const Hand &h) {
    return stream << "Information on Hand object:" << endl
                  << "Number of cards in Hand: " << h.getCards().size() << endl;
}

Hand::~Hand() {
    for (auto p : cards) {
        delete p;
        p = nullptr;
    }
}

// TODO: remove maybe ?
Card * Hand::getRandomCard() {
    int randomCardIndex = rand() % cards.size();
    return cards.at(randomCardIndex);
}

const vector<Card *> &Hand::getCards() const {
    return cards;
}

void Hand::setCards(const vector<Card *> &cards) {
    Hand::cards = cards;
}

void Hand::addCard(Card *card) {
    cout << "Adding card to hand" << endl;
    cards.push_back(card);
}

void Hand::removeCard(int index) {
    cout << "Removing card from hand" << endl;
    cards.erase(cards.begin() + index);
}

bool Hand::removeCard(Card* card) {
    auto position = find(cards.begin(), cards.end(), card);
    if (position != cards.end()) {
        cards.erase(position);
        // TODO: put back to singleton Deck
        return true;
    }
    cout << "Error deleting order." << endl;
    return false;
}


int Hand::getAmountOfCardsOfType(Card::CardType type) {
    int counter = 0;
    for (Card *card: cards)
        if (card->getType() == type) counter++;

    return counter;
}