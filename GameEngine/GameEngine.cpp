#include "GameEngine.h"

GameEngine::GameEngine(Map *map, Deck *deck, vector<Player *> &players) : map(map), deck(deck),
                                                                          players(players) {}
