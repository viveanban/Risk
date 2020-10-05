//
// Created by tarek ait hamouda on 2020-10-04.
//

#ifndef RISK_CARDS_H
#define RISK_CARDS_H

#include <vector>

using namespace std;

enum cardType {
    bomb = 0,
    reinforcement = 1,
    blockade = 2,
    airlift = 3,
    diplomacy = 4
};

class Card {
private:
    cardType type;
public:
    Card();
    Card(cardType type);
    void play();
};

class Deck {
private:
    vector<Card*> cards;
public:
    Deck();
    Deck(vector<Card*> cards);
    Card& draw();
};

#endif //RISK_CARDS_H
