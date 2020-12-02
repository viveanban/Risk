#include "PlayerStrategies.h"
#include "../GameEngine/GameEngine.h"
#include <algorithm>

// DEFAULT PLAYER STRATEGY
const int PlayerStrategy::PERCENTAGE = 90;

PlayerStrategy::PlayerStrategy() {}

PlayerStrategy::PlayerStrategy(Player *player) : player(player) {}

PlayerStrategy::~PlayerStrategy() = default;

void PlayerStrategy::issueDeployOrder() {
    // Reinforcement card
    playReinforcementCard();

    // Deploy order
    auto *deployOrder = new DeployOrder(player);
    bool successful = setUpDeployOrder(deployOrder);
    if (!successful) {
        delete deployOrder;
        deployOrder = nullptr;
    } else {
        GameEngine::getInstance()->getGameState()->updateGameState(player, issuing_orders, deployOrder, nullptr);
    }
}

// TODO: return false when random player will be deleted (Viveka)
bool PlayerStrategy::setUpDeployOrder(DeployOrder *order) {
    // This ensures that the numberOfArmiesToDeploy is always smaller or equal than numberOfArmiesInReinforcementPool
    order->setNumberOfArmiesToDeploy(getUnitNumberToDeploy());

    // Set the target territory to be player's territory with the least amount of unit armies
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    order->setTargetTerritory(territoriesToDefend.at(0));

    // Update the priority of the target territory so that it is not at the top of the list for the next deploy order
    order->getTargetTerritory()->setPriority(
            order->getTargetTerritory()->getPriority() + order->getNumberOfArmiesToDeploy());

    // Update number of armies
    player->setNumberOfArmiesInReinforcementPool(
            player->getNumberofArmiesInReinforcementPool() - order->getNumberOfArmiesToDeploy());

    // Update order list
    player->getOrders()->add(order);

    return true;
}

void PlayerStrategy::playReinforcementCard() {
    for (Card *card: player->getHandOfCards()->getCards()) {
        if (card->getType() == Card::reinforcement) {
            bool playReinforcementCard = rand() % 2;
            if (playReinforcementCard) {
                player->setNumberOfArmiesInReinforcementPool(player->getNumberofArmiesInReinforcementPool() + 5);
                player->getHandOfCards()->removeCard(card);
                GameEngine::getInstance()->getGameState()->updateGameState(player, issuing_orders, nullptr, card);
            }
            break;
        }
    }
}

void PlayerStrategy::issueAdvanceOrder() {
    auto *advanceOrder = new AdvanceOrder(player);
    bool successful = setUpAdvanceOrder(advanceOrder);
    if (!successful) {
        delete advanceOrder;
        advanceOrder = nullptr;
    } else {
        GameEngine::getInstance()->getGameState()->updateGameState(player, issuing_orders, advanceOrder, nullptr);
    }
}

// TODO: return false when random player will be deleted (Viveka)
bool PlayerStrategy::setUpAdvanceOrder(AdvanceOrder *order) {
    // Determine src territory
    order->setSourceTerritory(player->getTerritories().at(rand() % player->getTerritories().size()));
    bool attack = dynamic_cast<AggressivePlayerStrategy *> (player->getStrategy()) != nullptr;
    order->setAdvanceOrderType(attack ? AdvanceOrderType::attack : AdvanceOrderType::transfer);

    // Determine target territory
    vector<Territory *> territoriesToChooseFrom = attack ? player->toAttack(order->getSourceTerritory())
                                                         : player->toDefend(order->getSourceTerritory());
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to " << (attack ? "attack" : "transfer") << endl;
        return false;
    }

    order->setTargetTerritory(territoriesToChooseFrom.at(0));

    // Determine number of territories to advance
    int numberOfArmies = (rand() %
                          (order->getSourceTerritory()->getPriority() > 0 ? order->getSourceTerritory()->getPriority()
                                                                          : 6)) + 1;
    order->setNumberOfArmiesToAdvance(numberOfArmies);

    // Update priority
    order->getSourceTerritory()->setPriority(order->getSourceTerritory()->getPriority() - numberOfArmies);
    order->getTargetTerritory()->setPriority(attack ?
                                             order->getTargetTerritory()->getPriority() - numberOfArmies :
                                             order->getTargetTerritory()->getPriority() + numberOfArmies);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

void PlayerStrategy::issueOrderFromCard(Card *cardChosen) {
    Order *order = cardChosen->play();
    if (order) {
        order->setPlayer(player);

        bool successful = false;

        if (order->getName() == "Airlift") {
            successful = issueAirliftOrder((AirliftOrder *) order);
        } else if (order->getName() == "Negotiate") {
            successful = issueNegotiateOrder((NegotiateOrder *) order);
        } else if (order->getName() == "Bomb") {
            successful = issueBombOrder((BombOrder *) order);
        } else if (order->getName() == "Blockade") {
            successful = issueBlockadeOrder((BlockadeOrder *) order);
        }

        if (!successful) {
            delete order;
            order = nullptr;
        } else {
            GameEngine::getInstance()->getGameState()->updateGameState(player, issuing_orders, order, cardChosen);
            player->getHandOfCards()->removeCard(cardChosen);
        }
    }
}

// TODO: return false when random player will be deleted (Viveka)
bool PlayerStrategy::issueBombOrder(BombOrder *order) {
    // Randomly determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to attack." << endl;
        return false;
    }

    Territory *targetTerritory = territoriesToAttack.at(
            territoriesToAttack.size() - 1); // Attack the strongest enemy territory
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() / 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

// TODO: return false when random player will be deleted (Viveka)
bool PlayerStrategy::issueBlockadeOrder(BlockadeOrder *order) {
    // Determine target territory to be the player's territory with the most army units
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    Territory *targetTerritory = territoriesToDefend.at(territoriesToDefend.size() - 1);
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() * 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

// TODO: just return false at the end when random player will be deleted (Viveka)
bool PlayerStrategy::issueAirliftOrder(AirliftOrder *order) {
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine src territory
    Territory *sourceTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());
    order->setSourceTerritory(sourceTerritory);

    // Determine target territory
    Territory *targetTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());
    order->setTargetTerritory(targetTerritory);

    // Determine number of armies to airlift
    int numberOfArmiesToAirlift =
            (rand() %
             (order->getSourceTerritory()->getPriority() > 0 ? order->getSourceTerritory()->getPriority() : 6)) + 1;
    order->setNumberOfArmiesToAirlift(numberOfArmiesToAirlift);

    // Update priority
    sourceTerritory->setPriority(order->getSourceTerritory()->getPriority() - numberOfArmiesToAirlift);
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() + numberOfArmiesToAirlift);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

// TODO: return false when random player will be deleted (Viveka)
bool PlayerStrategy::issueNegotiateOrder(NegotiateOrder *order) {
    // Determine a random enemy player
    vector<Player *> players = GameEngine::getInstance()->getPlayers();
    if (players.size() < 2) {
        cout << "Cannot play a negotiate order with only one player!" << endl;
        return false;
    }

    // Determine target player
    do {
        order->setTargetPlayer(players.at(rand() % players.size()));
    } while (order->getTargetPlayer() == player);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool PlayerStrategy::getBooleanInput(string printStatement) {
    bool output = false;
    do {
        cout << printStatement << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> boolalpha >> output;
    } while (cin.fail());
    return output;
}

int PlayerStrategy::getIntegerInput(string printStatement, int leftBound, int rightBound) {
    int output = 0;
    do {
        cout << printStatement << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> output;
    } while (cin.fail() or output < leftBound or output >= rightBound);
    return output;
}

int PlayerStrategy::getUnitNumberToDeploy() {
    int totalAvailableArmies = player->getNumberofArmiesInReinforcementPool();
    return (rand() % totalAvailableArmies) + 1;
}

// Shallow copy because we don't want to create new players
PlayerStrategy::PlayerStrategy(const PlayerStrategy &original) {
    this->player = original.player;
}

PlayerStrategy &PlayerStrategy::operator=(const PlayerStrategy &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

std::ostream &operator<<(ostream &stream, PlayerStrategy &playerStrategy) {
    return stream << playerStrategy.player->getPlayerName() << " uses the base player strategy." << endl;
}

// HUMAN PLAYER STRATEGY
HumanPlayerStrategy::HumanPlayerStrategy() {}

HumanPlayerStrategy::HumanPlayerStrategy(Player *player) {
    this->player = player;
}

HumanPlayerStrategy::~HumanPlayerStrategy() = default;

// Shallow copy because we don't want to create new players
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &original) {
    this->player = original.player;
}

HumanPlayerStrategy &HumanPlayerStrategy::operator=(const HumanPlayerStrategy &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

std::ostream &operator<<(ostream &stream, HumanPlayerStrategy &playerStrategy) {
    return stream << playerStrategy.player->getPlayerName() << " uses the human player strategy." << endl;
}

vector<Territory *> HumanPlayerStrategy::toAttack() {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    // don't need to sort for human player because they can choose any from list.
    return territoriesToAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend() {
    // don't need to sort for human player because they can choose any from list.
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;
    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }
    // don't need to sort for human player because they can choose any from list.
    return territoriesToAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    // don't need to sort for human player because they can choose any from list.
    return territoriesToDefend;
}

bool HumanPlayerStrategy::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (this->player->getNumberofArmiesInReinforcementPool() > 0) {
        issueDeployOrder();
        return true;
    } else { // Other orders
        bool continueIssuingOrders = getBooleanInput("Do you want to continue issuing orders? [true/false] ");
        if (continueIssuingOrders) {
            if (player->getHandOfCards()->getCards().empty()) {
                cout << "You must issue an Advance Order because your hand is empty!" << endl;
                issueAdvanceOrder();
            } else {
                vector<string> typesOfMove = {"Advance", "Pick a card"};
                for (int i = 0; i < typesOfMove.size(); i++) {
                    cout << i << " - " << typesOfMove.at(i) << endl;
                }
                int chosenTypeOfMove = PlayerStrategy::getIntegerInput("Please choose your next type of move: ", 0,
                                                                       typesOfMove.size());

                if (chosenTypeOfMove == 0)
                    issueAdvanceOrder();
                else if (chosenTypeOfMove == 1) {
                    // Print all cards in Hand
                    vector<Card *> handOfCards = player->getHandOfCards()->getCards();
                    for (int i = 0; i < handOfCards.size(); ++i) {
                        cout << i << " - " << handOfCards.at(i)->getTypeName() << endl;
                    }
                    // Pick a card
                    Card *cardChosen = handOfCards.at(getIntegerInput("Please select the card you wish to play: ",
                                                                      0, handOfCards.size()));
                    // Play card
                    issueOrderFromCard(cardChosen);
                }
            }
        }
        return continueIssuingOrders;
    }
}

bool HumanPlayerStrategy::setUpDeployOrder(DeployOrder *order) {
    int totalAvailableArmies = player->getNumberofArmiesInReinforcementPool();

    int numberOfArmiesToDeploy = PlayerStrategy::getIntegerInput(
            "Please enter the number of armies you wish to deploy (available troops " +
            to_string(totalAvailableArmies) + "): ", 0, totalAvailableArmies + 1);
    order->setNumberOfArmiesToDeploy(numberOfArmiesToDeploy);


    // Set the target territory to be player's territory with the least amount of unit armies
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    for (int i = 0; i < territoriesToDefend.size(); ++i) {
        cout << i << " - " << territoriesToDefend.at(i)->getTerritoryName() << " (unit nb: "
             << territoriesToDefend.at(i)->getUnitNbr() << ")" << endl;
    }
    Territory *targetTerritory = territoriesToDefend.at(
            PlayerStrategy::getIntegerInput("Please pick a target territory to deploy to: ",
                                            0, territoriesToDefend.size()));
    order->setTargetTerritory(targetTerritory);

    // Update the priority of the target territory so that it is not at the top of the list for the next deploy order
    order->getTargetTerritory()->setPriority(
            order->getTargetTerritory()->getPriority() + order->getNumberOfArmiesToDeploy());

    // Update number of armies
    player->setNumberOfArmiesInReinforcementPool(
            player->getNumberofArmiesInReinforcementPool() - numberOfArmiesToDeploy);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

void HumanPlayerStrategy::playReinforcementCard() {
    for (Card *card: player->getHandOfCards()->getCards()) {
        if (card->getType() == Card::reinforcement) {
            bool playReinforcementCard = PlayerStrategy::getBooleanInput(
                    "Would you like to play the reinforcement card? [true/false]");

            if (playReinforcementCard) {
                player->setNumberOfArmiesInReinforcementPool(player->getNumberofArmiesInReinforcementPool() + 5);
                player->getHandOfCards()->removeCard(card);
                GameEngine::getInstance()->getGameState()->updateGameState(player, issuing_orders, nullptr, card);
            }

            break;
        }
    }
}

bool HumanPlayerStrategy::setUpAdvanceOrder(AdvanceOrder *order) {
    // Determine src territory
    auto territories = player->getTerritories();
    for (int i = 0; i < territories.size(); ++i) {
        cout << i << " - " << territories.at(i)->getTerritoryName() << " (unit nb: "
             << territories.at(i)->getUnitNbr()
             << ")" << endl;
    }
    Territory *sourceTerritory = territories.at(
            PlayerStrategy::getIntegerInput("Please pick a source territory to advance from: ", 0,
                                            territories.size()));
    order->setSourceTerritory(sourceTerritory);

    // Pick attack or defend
    bool attack = PlayerStrategy::getBooleanInput("Do you wish to attack a territory? [true/false] ");
    order->setAdvanceOrderType(attack ? AdvanceOrderType::attack : AdvanceOrderType::transfer);

    vector<Territory *> territoriesToChooseFrom = attack ?
                                                  player->toAttack(sourceTerritory) :
                                                  player->toDefend(sourceTerritory);
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to " << (attack ? "attack" : "transfer") << endl;
        return false;
    }

    // Determine target territory
    for (int i = 0; i < territoriesToChooseFrom.size(); ++i) {
        cout << i << " - " << territoriesToChooseFrom.at(i)->getTerritoryName() << " (unit nb: "
             << territoriesToChooseFrom.at(i)->getUnitNbr() << ")" << endl;
    }
    Territory *targetTerritory = territoriesToChooseFrom.at(
            PlayerStrategy::getIntegerInput("Please pick a target territory to advance to: ", 0,
                                            territoriesToChooseFrom.size()));
    order->setTargetTerritory(targetTerritory);

    // Determine number of territories to advance
    int numberOfArmies = PlayerStrategy::getIntegerInput(
            "Please enter the number of armies you wish to advance (available troops " +
            to_string(sourceTerritory->getPriority()) + "): ", 0, sourceTerritory->getPriority() + 1);
    order->setNumberOfArmiesToAdvance(numberOfArmies);


    // Update priority
    order->getSourceTerritory()->setPriority(order->getSourceTerritory()->getPriority() - numberOfArmies);
    order->getTargetTerritory()->setPriority(attack ?
                                             order->getTargetTerritory()->getPriority() - numberOfArmies :
                                             order->getTargetTerritory()->getPriority() + numberOfArmies);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool HumanPlayerStrategy::issueBombOrder(BombOrder *order) {
    // Randomly determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to attack." << endl;
        return false;
    }

    // Determine target territory
    for (int i = 0; i < territoriesToAttack.size(); ++i) {
        cout << i << " - " << territoriesToAttack.at(i)->getTerritoryName() << " (unit nb: "
             << territoriesToAttack.at(i)->getUnitNbr() << ")" << endl;
    }
    Territory *targetTerritory = territoriesToAttack.at(
            PlayerStrategy::getIntegerInput("Please pick a target territory to bomb: ", 0,
                                            territoriesToAttack.size()));
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(targetTerritory->getPriority() / 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool HumanPlayerStrategy::issueBlockadeOrder(BlockadeOrder *order) {
    // Determine target territory to be the player's territory with the most army units
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine target territory
    for (int i = 0; i < territoriesToDefend.size(); ++i) {
        cout << i << " - " << territoriesToDefend.at(i)->getTerritoryName() << " (unit nb: "
             << territoriesToDefend.at(i)->getUnitNbr() << ")" << endl;
    }
    Territory *targetTerritory = territoriesToDefend.at(
            PlayerStrategy::getIntegerInput("Please pick a target territory to blockade: ", 0,
                                            territoriesToDefend.size()));

    // Update priority
    targetTerritory->setPriority(targetTerritory->getPriority() * 2);

    order->setTargetTerritory(targetTerritory);
    // Update order list
    player->getOrders()->add(order);
    return true;
}

bool HumanPlayerStrategy::issueAirliftOrder(AirliftOrder *order) {
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine src territory
    for (int i = 0; i < territoriesToDefend.size(); ++i) {
        cout << i << " - " << territoriesToDefend.at(i)->getTerritoryName() << " (unit nb: "
             << territoriesToDefend.at(i)->getUnitNbr() << ")" << endl;
    }
    Territory *sourceTerritory = territoriesToDefend.at(
            PlayerStrategy::getIntegerInput("Please pick a source territory to airlift from: ", 0,
                                            territoriesToDefend.size()));
    order->setSourceTerritory(sourceTerritory);

    // Determine target territory
    Territory *targetTerritory = territoriesToDefend.at(
            PlayerStrategy::getIntegerInput("Please pick a target territory to airlift to: ", 0,
                                            territoriesToDefend.size()));
    order->setTargetTerritory(targetTerritory);

    // Determine number of armies to airlift
    int numberOfArmiesToAirlift = PlayerStrategy::getIntegerInput(
            "Please enter the number of armies you wish to airlift (available troops " +
            to_string(sourceTerritory->getPriority()) + "): ", 0, sourceTerritory->getPriority() + 1);
    order->setNumberOfArmiesToAirlift(numberOfArmiesToAirlift);

    // Update priority
    sourceTerritory->setPriority(sourceTerritory->getPriority() - numberOfArmiesToAirlift);
    targetTerritory->setPriority(targetTerritory->getPriority() + numberOfArmiesToAirlift);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool HumanPlayerStrategy::issueNegotiateOrder(NegotiateOrder *order) {
    // Determine a random enemy player
    vector<Player *> players = GameEngine::getInstance()->getPlayers();
    if (players.size() < 2) {
        cout << "Cannot play a negotiate order with only one player!" << endl;
        return false;
    }

    // Determine target player
    for (int i = 0; i < players.size(); ++i) {
        cout << i << " - " << players.at(i)->getPlayerName() << " (nb of territories owned: "
             << players.at(i)->getTerritories().size() << ")" << endl;
    }

    do {
        order->setTargetPlayer(players.at(
                PlayerStrategy::getIntegerInput("Please pick a target player to negociate a diplomacy with: ", 0,
                                                players.size())));
    } while (order->getTargetPlayer() == player);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

// AGGRESIVE PLAYER STRATEGY
AggressivePlayerStrategy::AggressivePlayerStrategy() {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) {
    this->player = player;
}

AggressivePlayerStrategy::~AggressivePlayerStrategy() = default;

// Shallow copy because we don't want to create new players
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &original) {
    this->player = original.player;
}

AggressivePlayerStrategy &AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

std::ostream &operator<<(ostream &stream, AggressivePlayerStrategy &playerStrategy) {
    return stream << playerStrategy.player->getPlayerName() << " uses the aggressive player strategy." << endl;
}

bool AggressivePlayerStrategy::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (player->getNumberofArmiesInReinforcementPool() > 0) {
        issueDeployOrder();
        return true;
    } else { // Other orders
        int percentage = (rand() % 100) + 1;
        bool continueIssuingOrders = percentage <= PERCENTAGE;
        if (continueIssuingOrders) {
            Card *bombCard = player->getHandOfCards()->getBombCard();
            bool advance = !bombCard || rand() % 2;
            if (advance) { //Always issue an Advance order if player has an empty hand
                issueAdvanceOrder();
            } else {
                // Play card
                issueOrderFromCard(bombCard);
            }
        }
        return continueIssuingOrders;
    }
}

bool AggressivePlayerStrategy::setUpDeployOrder(DeployOrder *order) {
    // This ensures that the numberOfArmiesToDeploy is always smaller or equal than numberOfArmiesInReinforcementPool
    order->setNumberOfArmiesToDeploy(getUnitNumberToDeploy());

    // Set the target territory to be player's territory with the least amount of unit armies
    vector<Territory *> territoriesToDefend = player->toDefend();

    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    Territory* targetTerritory;
    for(Territory* territory: territoriesToDefend) {
        if(!toAttack(territory).empty()) {
            targetTerritory = territory;
            break;
        }
    }

    order->setTargetTerritory(targetTerritory ? targetTerritory : territoriesToDefend.at(0));

    // Update the priority of the target territory so that it is not at the top of the list for the next deploy order
    order->getTargetTerritory()->setPriority(
            order->getTargetTerritory()->getPriority() + order->getNumberOfArmiesToDeploy());

    // Update number of armies
    player->setNumberOfArmiesInReinforcementPool(
            player->getNumberofArmiesInReinforcementPool() - order->getNumberOfArmiesToDeploy());

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool AggressivePlayerStrategy::setUpAdvanceOrder(AdvanceOrder *order) {
    order->setAdvanceOrderType(AdvanceOrderType::attack);

    // Determine src territory: Aggressive player wants to attack with the their strongest country
    order->setSourceTerritory(player->toDefend().at(0));

    // Determine target territory: Aggressive player wants to attack the weakest targetTerritory
    vector<Territory *> territoriesToChooseFrom = player->toAttack(order->getSourceTerritory());
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to attack" << endl;
        return false;
    }
    order->setTargetTerritory(territoriesToChooseFrom.at(0));

    order->setNumberOfArmiesToAdvance(order->getSourceTerritory()->getPriority());

    // Update priority
    order->getSourceTerritory()->setPriority(
            order->getSourceTerritory()->getPriority() - order->getNumberOfArmiesToAdvance());
    order->getTargetTerritory()->setPriority(
            order->getTargetTerritory()->getPriority() - order->getNumberOfArmiesToAdvance());

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool AggressivePlayerStrategy::issueBombOrder(BombOrder *order) {
    // Determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to attack." << endl;
        return false;
    }

    Territory *targetTerritory = territoriesToAttack.at(
            territoriesToAttack.size() - 1); // Attack the strongest enemy territory
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() / 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

vector<Territory *> AggressivePlayerStrategy::toAttack() {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }

    sort(territoriesToAttack.begin(), territoriesToAttack.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return territoriesToAttack;
}

vector<Territory *> AggressivePlayerStrategy::toDefend() {
    sort(player->getTerritories().begin(), player->getTerritories().end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() > rhs->getPriority();
    });

    return player->getTerritories();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }

    sort(territoriesToAttack.begin(), territoriesToAttack.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return territoriesToAttack;
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }

    sort(territoriesToDefend.begin(), territoriesToDefend.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() > rhs->getPriority();
    });

    return territoriesToDefend;
}

int AggressivePlayerStrategy::getUnitNumberToDeploy() {
    int totalAvailableArmies = player->getNumberofArmiesInReinforcementPool();
    return totalAvailableArmies;
}

// BENEVOLENT PLAYER STRATEGY
BenevolentPlayerStrategy::BenevolentPlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) {
    this->player = player;
}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() = default;

// Shallow copy because we don't want to create new players
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &original) {
    this->player = original.player;
}

BenevolentPlayerStrategy &BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

std::ostream &operator<<(ostream &stream, BenevolentPlayerStrategy &playerStrategy) {
    return stream << playerStrategy.player->getPlayerName() << " uses the benevolent player strategy." << endl;
}

bool BenevolentPlayerStrategy::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (this->player->getNumberofArmiesInReinforcementPool() > 0) {
        issueDeployOrder();
        return true;
    } else {
        //Since benevolent player cannot conquer a territory, he won't have a card, he can just move to weaker territories.
        int percentage = (rand() % 100) + 1;
        bool continueIssuingOrders = percentage <= PERCENTAGE;
        if (continueIssuingOrders) {
            issueAdvanceOrder();
        }
        return continueIssuingOrders;
    }
}

bool BenevolentPlayerStrategy::setUpAdvanceOrder(AdvanceOrder *order) {
    order->setAdvanceOrderType(AdvanceOrderType::transfer);

    // Determine target territory: Benevolent player will transfer to their weakest country
    vector<Territory *> choicesOfTargetTerritories = player->toDefend();
    int chosenTerritoryIndex = 0;

    // Determine source territory: Benevolent player will transfer from their strongest adjacent country
    vector<Territory *> choicesOfSourceTerritories = player->toDefend(
            choicesOfTargetTerritories.at(chosenTerritoryIndex));

    // Go through loop until you can find a territory that could transfer some army to the weakest country possible
    while (choicesOfSourceTerritories.empty() && chosenTerritoryIndex < choicesOfTargetTerritories.size() - 1) {
        chosenTerritoryIndex++;
        choicesOfSourceTerritories = player->toDefend(choicesOfTargetTerritories.at(chosenTerritoryIndex));
    }

    if (choicesOfSourceTerritories.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories than could transfer to its weakest territory" << endl;
        return false;
    }

    order->setTargetTerritory(choicesOfTargetTerritories.at(
            chosenTerritoryIndex)); // Weakest territory where territories can be transferred to
    order->setSourceTerritory(choicesOfSourceTerritories.at(
            choicesOfSourceTerritories.size() - 1)); // Strongest adjacent territory to transfer from


    // Determine number of territories to advance
    int numberOfArmies = (rand() %
                          (order->getSourceTerritory()->getPriority() > 0 ? order->getSourceTerritory()->getPriority()
                                                                          : 6)) + 1;
    order->setNumberOfArmiesToAdvance(numberOfArmies);

    // Update priority
    order->getSourceTerritory()->setPriority(order->getSourceTerritory()->getPriority() - numberOfArmies);
    order->getTargetTerritory()->setPriority(order->getTargetTerritory()->getPriority() + numberOfArmies);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

vector<Territory *> BenevolentPlayerStrategy::toAttack() {
    return {};
}

vector<Territory *> BenevolentPlayerStrategy::toDefend() {
    // Need to return territories in order of weakest --> strongest
    sort(player->getTerritories().begin(), player->getTerritories().end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return player->getTerritories();
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Territory *srcTerritory) {
    return {};
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    //sort from weakest to strongest
    sort(player->getTerritories().begin(), player->getTerritories().end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return territoriesToDefend;
}

int BenevolentPlayerStrategy::getUnitNumberToDeploy() {
    int totalAvailableArmies = player->getNumberofArmiesInReinforcementPool();
    return (rand() % ((totalAvailableArmies / 2) == 0 ? totalAvailableArmies : (totalAvailableArmies / 2))) + 1;
}


// NEUTRAL PLAYER STRATEGY
NeutralPlayerStrategy::NeutralPlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) {
    this->player = player;
}

NeutralPlayerStrategy::~NeutralPlayerStrategy() = default;

// Shallow copy because we don't want to create new players
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &original) {
    this->player = original.player;
}

NeutralPlayerStrategy &NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy &other) {
    if (this != &other) {
        this->player = other.player;
    }
    return *this;
}

std::ostream &operator<<(ostream &stream, NeutralPlayerStrategy &playerStrategy) {
    return stream << playerStrategy.player->getPlayerName() << " uses the neutral player strategy." << endl;
}

bool NeutralPlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> NeutralPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toDefend() {
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
}

//====================================== Implemented for testing purposes ==============================================
// RANDOM STRATEGY

RandomPlayerStrategy::RandomPlayerStrategy(Player *player) {
    this->player = player;
}

bool RandomPlayerStrategy::issueOrder() {
    // Issue deploy orders as long as player's reinforcement pool is not empty
    if (player->getNumberofArmiesInReinforcementPool() > 0) {
        issueDeployOrder();
        return true;
    } else { // Other orders
        int percentage = (rand() % 100) + 1;
        bool continueIssuingOrders = percentage <= PERCENTAGE;
        if (continueIssuingOrders) {
            bool advance = player->getHandOfCards()->getCards().empty() || rand() % 2;
            if (advance) { //Always issue an Advance order if player has an empty hand
                issueAdvanceOrder();
            } else {
                // Pick a card
                Card *cardChosen = player->getHandOfCards()->getNextCard();
                if (!cardChosen) return continueIssuingOrders; // if the reinforcement card was picked, just continue...

                // Play card
                issueOrderFromCard(cardChosen);
            }
        }
        return continueIssuingOrders;
    }
}

bool RandomPlayerStrategy::setUpDeployOrder(DeployOrder *order) {
    return PlayerStrategy::setUpDeployOrder(order);
}

bool RandomPlayerStrategy::setUpAdvanceOrder(AdvanceOrder *order) {
    // Determine src territory
    order->setSourceTerritory(player->getTerritories().at(rand() % player->getTerritories().size()));

    // Determine target territory
    bool attack = rand() % 2;
    order->setAdvanceOrderType(attack ? AdvanceOrderType::attack : AdvanceOrderType::transfer);

    vector<Territory *> territoriesToChooseFrom = attack ? player->toAttack(order->getSourceTerritory())
                                                         : player->toDefend(
                    order->getSourceTerritory());
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to " << (attack ? "attack" : "transfer") << endl;
        return false;
    }

    order->setTargetTerritory(territoriesToChooseFrom.at(0));

    // Determine number of armies to advance
    int armiesToAdvance = (rand() %
                           (order->getSourceTerritory()->getPriority() > 0 ? order->getSourceTerritory()->getPriority()
                                                                           : 6)) + 1;
    order->setNumberOfArmiesToAdvance(armiesToAdvance);

    // Update priority
    order->getSourceTerritory()->setPriority(
            order->getSourceTerritory()->getPriority() - order->getNumberOfArmiesToAdvance());
    order->getTargetTerritory()->setPriority(attack ?
                                             order->getTargetTerritory()->getPriority() -
                                             order->getNumberOfArmiesToAdvance() :
                                             order->getTargetTerritory()->getPriority() +
                                             order->getNumberOfArmiesToAdvance());

    // Update order list
    player->getOrders()->add(order);

    return true;
}

vector<Territory *> RandomPlayerStrategy::toAttack() {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: GameEngine::getInstance()->getMap()->getTerritoryList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }

    sort(territoriesToAttack.begin(), territoriesToAttack.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return territoriesToAttack;
}

vector<Territory *> RandomPlayerStrategy::toDefend() {
    sort(player->getTerritories().begin(), player->getTerritories().end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return player->getTerritories();
}

vector<Territory *> RandomPlayerStrategy::toAttack(Territory *srcTerritory) {
    vector<Territory *> territoriesToAttack;

    for (Territory *territory: srcTerritory->getAdjList()) {
        if (territory->getOwner() != this->player)
            territoriesToAttack.push_back(territory);
    }

    sort(territoriesToAttack.begin(), territoriesToAttack.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return territoriesToAttack;
}

vector<Territory *> RandomPlayerStrategy::toDefend(Territory *srcTerritory) {
    vector<Territory *> territoriesToDefend;
    for (Territory *adjacentTerritory: srcTerritory->getAdjList()) {
        if (adjacentTerritory->getOwner() == this->player)
            territoriesToDefend.push_back(adjacentTerritory);
    }
    sort(territoriesToDefend.begin(), territoriesToDefend.end(), [](Territory *lhs, Territory *rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
    return territoriesToDefend;
}
