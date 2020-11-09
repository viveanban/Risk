#ifndef RISK_CARDS_H
#define RISK_CARDS_H

#include <vector>
#include <ostream>
#include <map>
#include "../Orders/Orders.h"

using namespace std;

/**
 * This class represents a card in the Risk game
 * A card has one of the following type: bomb, reinforcement, blockade, airlift and diplomacy
 */
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

    Card &operator=(const Card &otherCard);

    friend std::ostream &operator<<(std::ostream &stream, const Card &c);

    CardType getType() const;

    void setType(CardType type);

    /**
     * This method should create an order based on the card type and
     * return it to be used in the issueOrder method of player
     */
    Order *play();

private:
    CardType type;
};

/**
 * The Deck class represents the set of card available to be used during a game
 * It randomly picks cards from different type and add them to the stack of card to be used
 */
class Deck {
private:
    vector<Card *> cards;
public:
    Deck(int size);

    ~Deck();

    Deck(const Deck &original);

    Deck &operator=(const Deck &otherDeck);

    friend std::ostream &operator<<(std::ostream &stream, const Deck &d);

    const vector<Card *> &getCards() const;

    void setCards(const vector<Card *> &cards);

    void addCard(Card *card);

    /**
     * The draw method should randomly pick a card inside the deck, remove it
     * and return it to be used by the caller
     * @return a random card inside the deck
     */
    Card *draw();
};

/**
 * The Hand class represents the hand of card that a player has during a game
 * it contains a set of cards that the play can use
 */
class Hand {
private:
    vector<Card *> cards;
public:
    Hand();

    ~Hand();

    Hand(vector<Card *> cards);

    Hand(const Hand &original);

    Hand &operator=(const Hand &otherHand);

    friend std::ostream &operator<<(std::ostream &stream, const Hand &c);

    int getAmountOfCardsOfType(Card::CardType type);

    int getCardNbr() const;

    void setCardNbr(int cardNbr);

    const vector<Card *> &getCards() const;

    void setCards(const vector<Card *> &cards);

    /**
     * addCard() adds a given card to the hand of cards
     * @param card
     */
    void addCard(Card *card);

    /**
     * remove card from the Hand
     * @param index representing the index of the card to be removed
     */
    void removeCard(int index);
};

#endif //RISK_CARDS_H
