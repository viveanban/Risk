//
// Created by tarek ait hamouda on 2020-10-04.
//

#ifndef RISK_CARDS_H
#define RISK_CARDS_H

#include <vector>

using namespace std;

enum CardType {
    bomb = 0,
    reinforcement = 1,
    blockade = 2,
    airlift = 3,
    diplomacy = 4
};

class Card {
private:
    CardType type;
public:
    Card();

    Card(CardType type);

    Card(const Card &original);

    Card &operator=(const Card &otherTerritory);

    friend std::ostream &operator<<(std::ostream &stream, Card c);

    void setType(CardType type);

    CardType getType();

    void play();
};

class Deck {
private:
    vector<Card *> cards;
public:
    Deck();

    Deck(vector<Card *> cards);

    Deck(const Deck &original);

    ~Deck();

    Deck &operator=(const Deck &otherTerritory);

    friend std::ostream &operator<<(std::ostream &stream, Deck c);

    vector<Card *> getCards();

    void setCards(vector<Card *> cards);

    Card &draw();
};

#endif //RISK_CARDS_H
