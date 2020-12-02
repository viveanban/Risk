#include "PlayerStrategies.h"
#include "../GameEngine/GameEngine.h"
#include <algorithm>

// DEFAULT PLAYER STRATEGY
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

bool PlayerStrategy::setUpDeployOrder(DeployOrder *order) {
    int totalAvailableArmies = player->getNumberofArmiesInReinforcementPool();

    // This ensures that the numberOfArmiesToDeploy is always smaller or equal than numberOfArmiesInReinforcementPool
    order->setNumberOfArmiesToDeploy((rand() % totalAvailableArmies) + 1);

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

bool PlayerStrategy::setUpAdvanceOrder(AdvanceOrder *order) {
    // Determine src territory
    order->setSourceTerritory(player->getTerritories().at(rand() % player->getTerritories().size()));
    bool attack = rand() % 2;
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

        if(order->getName() == "Airlift") {
            successful = issueAirliftOrder((AirliftOrder*) order);
        }
        else if (order->getName() == "Negotiate") {
            successful = issueNegotiateOrder((NegotiateOrder*) order);
        }
        else if (order->getName() == "Bomb") {
            successful = issueBombOrder((BombOrder*) order);
        }
        else if (order->getName() == "Blockade") {
            successful = issueBlockadeOrder((BlockadeOrder*) order);
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

bool PlayerStrategy::issueBombOrder(BombOrder* order) {
    // Randomly determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to attack." << endl;
        return false;
    }

    Territory* targetTerritory = territoriesToAttack.at(rand() % territoriesToAttack.size());
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() / 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool PlayerStrategy::issueBlockadeOrder(BlockadeOrder* order) {
    // Determine target territory to be the player's territory with the most army units
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    Territory* targetTerritory = territoriesToDefend.at(territoriesToDefend.size() - 1);
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() * 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool PlayerStrategy::issueAirliftOrder(AirliftOrder* order) {
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine src territory
    Territory* sourceTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());
    order->setSourceTerritory(sourceTerritory);

    // Determine target territory
    Territory* targetTerritory = territoriesToDefend.at(rand() % territoriesToDefend.size());
    order->setTargetTerritory(targetTerritory);

    // Determine number of armies to airlift
    int numberOfArmiesToAirlift =
            (rand() % (order->getSourceTerritory()->getPriority() > 0 ? order->getSourceTerritory()->getPriority() : 6)) + 1;
    order->setNumberOfArmiesToAirlift(numberOfArmiesToAirlift);

    // Update priority
    sourceTerritory->setPriority(order->getSourceTerritory()->getPriority() - numberOfArmiesToAirlift);
    targetTerritory->setPriority(order->getTargetTerritory()->getPriority() + numberOfArmiesToAirlift);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool PlayerStrategy::issueNegotiateOrder(NegotiateOrder* order) {
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

// HUMAN PLAYER STRATEGY

HumanPlayerStrategy::HumanPlayerStrategy(Player *player) {
    this->player = player;
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

                if(chosenTypeOfMove == 0)
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
        cout << i << " - " << territoriesToDefend.at(i)->getTerritoryName() << " (unit nb: " << territoriesToDefend.at(i)->getPriority() << ")" << endl;
    }
    Territory* targetTerritory = territoriesToDefend.at(PlayerStrategy::getIntegerInput("Please pick a target territory to deploy to: ",
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
            bool playReinforcementCard = PlayerStrategy::getBooleanInput("Would you like to play the reinforcement card? [true/false]");

            if (playReinforcementCard) {
                player->setNumberOfArmiesInReinforcementPool(player->getNumberofArmiesInReinforcementPool() + 5);
                player->getHandOfCards()->removeCard(card);
                GameEngine::getInstance()->getGameState()->updateGameState(player, issuing_orders, nullptr, card);
            }

            break;
        }
    }
}

bool HumanPlayerStrategy::setUpAdvanceOrder(AdvanceOrder* order) {
    // Determine src territory
    auto territories = player->getTerritories();
    for (int i = 0; i < territories.size(); ++i) {
        cout << i << " - " << territories.at(i)->getTerritoryName() << " (unit nb: " << territories.at(i)->getPriority() << ")" << endl;
    }
    Territory* sourceTerritory = territories.at(PlayerStrategy::getIntegerInput("Please pick a source territory to advance from: ", 0,
                                                             territories.size()));
    order->setSourceTerritory(sourceTerritory);

    // Pick attack or defend
    bool attack = PlayerStrategy::getBooleanInput("Do you wish to attack a territory? [true/false] ");
    order->setAdvanceOrderType(attack ? AdvanceOrderType::attack : AdvanceOrderType::transfer);

    vector<Territory *> territoriesToChooseFrom = attack ? player->toAttack(sourceTerritory) : player->toDefend(
            sourceTerritory);
    if (territoriesToChooseFrom.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to " << (attack ? "attack" : "transfer") << endl;
        return false;
    }

    // Determine target territory
    for (int i = 0; i < territoriesToChooseFrom.size(); ++i) {
        cout << i << " - " << territoriesToChooseFrom.at(i)->getTerritoryName() << " (unit nb: " << territoriesToChooseFrom.at(i)->getPriority() << ")" << endl;
    }
    Territory* targetTerritory = territoriesToChooseFrom.at(PlayerStrategy::getIntegerInput("Please pick a target territory to advance to: ", 0,
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

bool HumanPlayerStrategy::issueBombOrder(BombOrder* order) {
    // Randomly determine a target territory to bomb
    vector<Territory *> territoriesToAttack = player->toAttack();
    if (territoriesToAttack.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to attack." << endl;
        return false;
    }

    // Determine target territory
    for (int i = 0; i < territoriesToAttack.size(); ++i) {
        cout << i << " - " << territoriesToAttack.at(i)->getTerritoryName() << " (unit nb: " << territoriesToAttack.at(i)->getPriority() << ")" << endl;
    }
    Territory* targetTerritory = territoriesToAttack.at(PlayerStrategy::getIntegerInput("Please pick a target territory to bomb: ", 0,
                                                                     territoriesToAttack.size()));
    order->setTargetTerritory(targetTerritory);

    // Update priority
    targetTerritory->setPriority(targetTerritory->getPriority() / 2);

    // Update order list
    player->getOrders()->add(order);

    return true;
}

bool HumanPlayerStrategy::issueBlockadeOrder(BlockadeOrder* order) {
    // Determine target territory to be the player's territory with the most army units
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine target territory
    for (int i = 0; i < territoriesToDefend.size(); ++i) {
        cout << i << " - " << territoriesToDefend.at(i)->getTerritoryName() << " (unit nb: " << territoriesToDefend.at(i)->getPriority() << ")" << endl;
    }
    Territory* targetTerritory = territoriesToDefend.at(
            PlayerStrategy::getIntegerInput("Please pick a target territory to blockade: ", 0, territoriesToDefend.size()));

    // Update priority
    targetTerritory->setPriority(targetTerritory->getPriority() * 2);

    // Update order list
    player->getOrders()->add(order);
    return true;
}

bool HumanPlayerStrategy::issueAirliftOrder(AirliftOrder* order) {
    vector<Territory *> territoriesToDefend = player->toDefend();
    if (territoriesToDefend.empty()) {
        cout << player->getPlayerName() << " could not issue order: " << order->getName()
             << " because this player has no territories to defend." << endl;
        return false;
    }

    // Determine src territory
    for (int i = 0; i < territoriesToDefend.size(); ++i) {
        cout << i << " - " << territoriesToDefend.at(i)->getTerritoryName() << " (unit nb: " << territoriesToDefend.at(i)->getPriority() << ")" << endl;
    }
    Territory* sourceTerritory = territoriesToDefend.at(
            PlayerStrategy::getIntegerInput("Please pick a source territory to airlift from: ", 0,
                                    territoriesToDefend.size()));
    order->setSourceTerritory(sourceTerritory);

    // Determine target territory
    Territory* targetTerritory = territoriesToDefend.at(
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

bool HumanPlayerStrategy::issueNegotiateOrder(NegotiateOrder* order) {
    // Determine a random enemy player
    vector<Player *> players = GameEngine::getInstance()->getPlayers();
    if (players.size() < 2) {
        cout << "Cannot play a negotiate order with only one player!" << endl;
        return false;
    }

    // Determine target player
    for (int i = 0; i < players.size(); ++i) {
        cout << i << " - " << players.at(i)->getPlayerName() << " (nb of territories owned: " << players.at(i)->getTerritories().size() << ")"  << endl;
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

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) {
    this->player = player;
}

bool AggressivePlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> AggressivePlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toDefend() {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
}

// BENEVOLENT PLAYER STRATEGY

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) {
    this->player = player;
}

bool BenevolentPlayerStrategy::issueOrder() {
    return false;
}

vector<Territory *> BenevolentPlayerStrategy::toAttack() {
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend() {
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Territory *srcTerritory) {
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Territory *srcTerritory) {
    return vector<Territory *>();
}


// NEUTRAL PLAYER STRATEGY

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) {
    this->player = player;
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
