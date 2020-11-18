#include "GameObservers.h"
#include <iomanip>
#include "./../Cards/Cards.h"

// SUBJECT
Subject::Subject() {
    observers = new list<Observer *>;
}

Subject::~Subject() {
    delete observers;
}

void Subject::attach(Observer *o) {
    observers->push_back(o);
};

void Subject::detach(Observer *o) {
    observers->remove(o);
};

void Subject::notify() {
    auto i = observers->begin();
    for (; i != observers->end(); ++i)
        (*i)->update();
};

//PHASE OBSERVER
PhaseObserver::PhaseObserver() : currentGameState{} {}

PhaseObserver::PhaseObserver(GameState *currGameState) : currentGameState(currGameState) {}

PhaseObserver::~PhaseObserver() {
    delete currentGameState;
    currentGameState = nullptr;
}

PhaseObserver::PhaseObserver(const PhaseObserver &original) {
    currentGameState = new GameState(*original.currentGameState);
}

PhaseObserver &PhaseObserver::operator=(const PhaseObserver &otherObserver) {
    currentGameState = new GameState(*otherObserver.currentGameState);
    return *this;
}

void PhaseObserver::update() {
    if (currentGameState->getCurrentPlayer() != nullptr) {
        displayPhaseUpdates();
    } else {
        cout << "Error occured while trying to update phase observer. Null value was found!" << endl;

    }
}

void PhaseObserver::displayPhaseUpdates() {
    cout << "===================================================================================================" << endl;
    cout << currentGameState->getCurrentPlayer()->getPlayerName() << ": " << getPhaseText() << " Phase." << endl;
    displaySpecialInformation();
    cout << "===================================================================================================" << endl;
}

string PhaseObserver::getPhaseText() {
    switch (currentGameState->getCurrentPhase()) {
        case reinforcement:
            return "Reinforcement";
        case issuing_orders:
            return "Issue orders";
        case orders_execution:
            return "Orders execution";
        default:
            return "Unknown State";
    }
}

void PhaseObserver::displaySpecialInformation() {
    switch (currentGameState->getCurrentPhase()) {
        case reinforcement:
            printReinforcementinfo();
            break;
        case issuing_orders:
        case orders_execution:
            printOrderInfo(currentGameState->getCurrentOrder(), currentGameState->getCurrentCard());
            break;
    }
}

void PhaseObserver::printReinforcementinfo() const {
    cout << currentGameState->getCurrentPlayer()->getPlayerName() << " has received new armies as part of reinforcement phase. He now owns "
         << currentGameState->getCurrentPlayer()->getNumberofArmies() << " number of armies." << endl;
}

void PhaseObserver::printOrderInfo(Order *order, Card *card) {
    if (card != nullptr && card->getType() == Card::CardType::reinforcement) {
        printReinforcementCardInfo();
        return;
    } else if (order == nullptr) {
        return;
    } else if (auto *deployOrder = dynamic_cast<DeployOrder *>(order)) {
        printDeployOrderInfo(deployOrder);
    } else if (auto *advanceOrder = dynamic_cast<AdvanceOrder *>(order)) {
        printAdvanceOrder(advanceOrder);
    } else if (auto *bombOrder = dynamic_cast<BombOrder *>(order)) {
        printBombOrder(bombOrder);
    } else if (auto *blockadeOrder = dynamic_cast<BlockadeOrder *>(order)) {
        printBlockadeOrder(blockadeOrder);
    } else if (auto *airliftOrder = dynamic_cast<AirliftOrder *>(order)) {
        printAirliftOrder(airliftOrder);
    } else if (auto *negotiateOrder = dynamic_cast<NegotiateOrder *>(order)) {
        printNegotiateOrder(negotiateOrder);
    } else {
        cout << "WARNING: Order of unknown type" << endl;
    }
}

void PhaseObserver::printBombOrder(BombOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued a Bomb order by playing a Bomb card on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a Bomb order on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " unit number." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printAdvanceOrder(AdvanceOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued an advance order from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to " << pOrder->getTargetTerritory()->getTerritoryName()
                 << " involving " << pOrder->getNumberOfArmiesToAdvance() << " number of armies." << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed an advance order from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to " << pOrder->getTargetTerritory()->getTerritoryName()
                 << " involving " << pOrder->getNumberOfArmiesToAdvance() << " number of armies." << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " is now owned by " <<
                 pOrder->getTargetTerritory()->getOwner()->getPlayerName() << " and has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies in it." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printDeployOrderInfo(DeployOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " issued a deploy order of "
                 << pOrder->getNumberOfArmiesToDeploy() << " armies on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            cout << currentGameState->getCurrentPlayer()->getNumberofArmies()
                 << " armies remaining in reinforcement pool."
                 << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a deploy order of "
                 << pOrder->getNumberOfArmiesToDeploy() << " armies on "
                 << pOrder->getTargetTerritory()->getTerritoryName()
                 << ". " << currentGameState->getCurrentPlayer()->getPlayerName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies on this territory." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printBlockadeOrder(BlockadeOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued a blockade order by using a blockade card on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;

            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a blockade order on "
                 << pOrder->getTargetTerritory()->getTerritoryName() << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies in it and is owned by "
                 << pOrder->getTargetTerritory()->getOwner()->getPlayerName() << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printAirliftOrder(AirliftOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued an Airlift order by using an Airlift card from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to "
                 << pOrder->getTargetTerritory()->getTerritoryName() << " with " << pOrder->getNumberOfArmiesToAirlift()
                 << " armies." << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed an Airlift order from "
                 << pOrder->getSourceTerritory()->getTerritoryName() << " to "
                 << pOrder->getTargetTerritory()->getTerritoryName() << " with " << pOrder->getNumberOfArmiesToAirlift()
                 << " armies." << endl;
            cout << pOrder->getTargetTerritory()->getTerritoryName() << " now has "
                 << pOrder->getTargetTerritory()->getUnitNbr() << " armies in it and is owned by "
                 << pOrder->getTargetTerritory()->getOwner()->getPlayerName() << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printNegotiateOrder(NegotiateOrder *pOrder) {
    switch (currentGameState->getCurrentPhase()) {
        case issuing_orders:
            cout << currentGameState->getCurrentPlayer()->getPlayerName()
                 << " issued a Negotiate order using a Diplomacy card with " <<
                 pOrder->getTargetPlayer()->getPlayerName() << endl;
            break;
        case orders_execution:
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " executed a Negotiate order with " <<
                 pOrder->getTargetPlayer()->getPlayerName() << endl;
            cout << currentGameState->getCurrentPlayer()->getPlayerName() << " and "
                 << pOrder->getTargetPlayer()->getPlayerName() << " cannot attack each other for this turn." << endl;
            break;
        default:
            break;
    }
}

void PhaseObserver::printReinforcementCardInfo() {
    cout << currentGameState->getCurrentPlayer()->getPlayerName() << " played a reinforcement card and has +5 armies. "
         << endl;
}

GameState::GameState(int totalTerritories, vector<Player *> *players, Player *currentPlayer, Phase currentPhase)
        : totalTerritories(totalTerritories), players(players),
          currentPlayer(currentPlayer), currentPhase(currentPhase) {}

GameState::GameState() : currentPlayer{}, currentPhase{}, players{}, totalTerritories{} {};

Player *GameState::getCurrentPlayer() const {
    return currentPlayer;
}

Phase GameState::getCurrentPhase() const {
    return currentPhase;
}

const vector<Player *> *GameState::getPlayers() const {
    return players;
}

int GameState::getTotalTerritories() const {
    return totalTerritories;
}

void GameState::setPlayers(vector<Player *> *players) {
    GameState::players = players;
}

void GameState::setCurrentPlayer(Player *currentPlayer) {
    GameState::currentPlayer = currentPlayer;
}

void GameState::setCurrentPhase(Phase currentPhase) {
    GameState::currentPhase = currentPhase;
}

void GameState::setTotalTerritories(int totalTerritories) {
    GameState::totalTerritories = totalTerritories;
}

// TODO: Handle a nullptr order??
void GameState::updateGameState(Player *player, Phase phase, Order *order, Card *card) {
    setCurrentPhase(phase);
    setCurrentPlayer(player);
    setCurrentOrder(order);
    setCurrentCard(card);
    notify();
}

GameState::GameState(const GameState &original) : Subject(original) {
    totalTerritories = original.totalTerritories;
    players = new vector<Player *>(*original.players);
    currentPlayer = new Player(*original.currentPlayer);
    currentPhase = original.currentPhase;
}

GameState &GameState::operator=(const GameState &original) {
    totalTerritories = original.totalTerritories;
    players = original.players;
    currentPlayer = original.currentPlayer;
    currentPhase = original.currentPhase;
    return *this;
}

GameState::~GameState() {
    players->clear();
    currentPlayer = nullptr;
}

Order *GameState::getCurrentOrder() const {
    return currentOrder;
}

void GameState::setCurrentOrder(Order *currentOrder) {
    GameState::currentOrder = currentOrder;
}

Card *GameState::getCurrentCard() const {
    return currentCard;
}

void GameState::setCurrentCard(Card *currentCard) {
    GameState::currentCard = currentCard;
}


//STATISTICS OBSERVER
StatisticsObserver::StatisticsObserver() : currentGameState{} {};

StatisticsObserver::StatisticsObserver(GameState *currGameState) : currentGameState(currGameState) {}

StatisticsObserver::~StatisticsObserver() {
    delete currentGameState;
    currentGameState = nullptr;
}

StatisticsObserver::StatisticsObserver(const StatisticsObserver &original) {
    currentGameState = new GameState(*original.currentGameState);
}

StatisticsObserver &StatisticsObserver::operator=(const StatisticsObserver &otherObserver) {
    currentGameState = new GameState(*otherObserver.currentGameState);
    return *this;
}

void StatisticsObserver::update() {
    this->displayStatsUpdate();
}

void StatisticsObserver::displayStatsUpdate() {
    cout << '|' << "Player" << setw(5) << '|' << "Territorial Control\t|" << endl;
    vector<float> playerDominationRatios{};
    for (Player *player: *currentGameState->getPlayers()) {
        float playerDomination = calculateWorldDomination(player->getTerritories().size());
        playerDominationRatios.push_back(playerDomination);
        cout << fixed << setprecision(2) << '|' << player->getPlayerName() << setw(3) << '|'
             << " % " << playerDomination << "\t\t|" << endl;
    }
    for (int i = 0; i < playerDominationRatios.size(); i++) {
        if (playerDominationRatios[i] == 100.0) {
            cout << "~ CONGRATULATIONS " << currentGameState->getPlayers()->at(i)->getPlayerName()
                 << " YOU WON THE GAME! VICCCTORY ~" << endl;
        }
    }
}

float StatisticsObserver::calculateWorldDomination(int numberOfTerritories) {
    return (float) numberOfTerritories / (float) currentGameState->getTotalTerritories() * 100;
}

