#ifndef RISK_CARDS_H
#define RISK_CARDS_H

#include <vector>
#include <ostream>

using namespace std;


class Card {
public:
    enum CardType {
        bomb,
        reinforcement,
        blockade,
        airlift,
        diplomacy
    };

    Card(CardType);

    Card(const Card &original);

    Card &operator=(const Card &otherTerritory);

    friend std::ostream &operator<<(std::ostream &stream, Card c);

    void setType(CardType type);

    CardType getType();

    void play();

private:
    CardType type;
};

class Deck {
private:
    vector<Card *> cards;
public:
    Deck(int size);

    ~Deck();

    Deck(const Deck &original);

    Deck &operator=(const Deck &otherTerritory);

    friend std::ostream &operator<<(std::ostream &stream, Deck c);

    const vector<Card *> &getCards() const;

    void setCards(const vector<Card *> &cards);

    void addCard(Card* card);

    Card* draw();
};

class Hand {
private:
    vector<Card *> cards;
    int cardNbr;
public:
    Hand();

    Hand(vector<Card*> cards);

    Hand(const Hand &original);

    int getCardNbr() const;

    void setCardNbr(int cardNbr);

    const vector<Card *> &getCards() const;

    void setCards(const vector<Card *> &cards);

    void removeCard(int index);
};

#endif //RISK_CARDS_H
