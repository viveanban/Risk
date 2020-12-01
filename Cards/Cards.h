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

    string getTypeName();

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
 * It randomly picks cards from different type and give them to the Players
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

    // Add card to the cards vector in Deck
    void addCard(Card *card);

    /**
     * The draw method should randomly pick a card inside the deck, remove it from the Deck
     * and return it to be used by the caller
     * @return a random card inside the deck
     */
    Card *draw();
};

/**
 * The Hand class represents the hand of cards that a player has during a game.
 */
class Hand {
private:
    vector<Card *> cards;
public:
    Hand();

    ~Hand();

    Hand(const Hand &original);

    Hand &operator=(const Hand &otherHand);

    friend std::ostream &operator<<(std::ostream &stream, const Hand &c);

    vector<Card *> &getCards();

    // Returns a card that is not a reinforcement card
    Card* getNextCard();

    // Add card to the cards vector in Hand
    void addCard(Card *card);

    // Remove a Card from the Hand
    void removeCard(Card* card);
};

#endif //RISK_CARDS_H
