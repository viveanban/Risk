#include <iostream>
#include "Player.h"

/**
 * Player Class implementation
 */

Player::Player() : playerName(), territories() {}//, cards() {}, orders() {}

// Copy Constructor
Player::Player(const Player &original) {
    playerName = original.playerName;
    territories = vector<Territory *>(original.territories.size());
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*original.territories[i]);

    // TODO: complete
}

Player &Player::operator=(const Player &otherPlayer) {
    playerName = otherPlayer.playerName;
    territories = vector<Territory *>(otherPlayer.territories.size());
    for (int i = 0; i < territories.size(); i++)
        territories[i] = new Territory(*otherPlayer.territories[i]);

    // TODO: complete

    return *this;
}

std::ostream &operator<<(std::ostream &stream, Player &player) {
    // TODO: complete
    return stream << "\tInformation on Player object:" << endl
                  << "\tPlayer Name: " << player.getPlayerName() << endl
                  << "\tNumber of Territories: " << player.getTerritories().size() << endl;
}

vector<Territory *> & Player::toDefend() {

}

vector<Territory *> & Player::toAttack() {

}

void Player::issueOrder() {

}

Player::~Player()
{
    // TODO: implement
}

// TODO: implement setters and getters