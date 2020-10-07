#include <iostream>
#include <ctime>
#include "Player.h"
#include <algorithm>

/**
 * Player Class implementation
 */

Player::Player() : playerName(), territories() {}//, cards(), orders()

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
                  << "\tNumber of Territories Owned: " << player.getTerritories().size() << endl;
}

vector<Territory *> Player::toDefend() {
    return territories;
}

vector<Territory *> Player::toAttack() {
    vector<Territory *> territoriesToAttack;
    for(auto territory: territories) {
        for(auto adjTerritory: territory->getAdjList()) {
            if(find(territories.begin(), territories.end(), adjTerritory) == territories.end() &&
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjTerritory) == territoriesToAttack.end())
                territoriesToAttack.push_back(adjTerritory);
        }
    }

    return territoriesToAttack;
}

// TODO: complete
void Player::issueOrder() {
    // Create Order object

    // Add OrdersList
}

string Player::getPlayerName() {
    return this->playerName;
}

void Player::setPlayerName(string playerName) {
    this->playerName = playerName;
}

vector<Territory *>& Player::getTerritories() {
    return this->territories;
}

void Player::setTerritories(vector<Territory *> &territories) {
    this->territories = territories;
}

// TODO: implement the other setters and getters

Player::~Player()
{
    // TODO: uncomment when merged orders and cards stuff
//    delete cards;
//    cards = NULL;
//    delete orders; // TODO: double-check if orders is to be deleted here
//    orders = NULL;

}
