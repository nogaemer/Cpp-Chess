//
// Created by Noah Schwenold on 1/8/2025.
//

#include "PlayerManager.h"


std::vector<Player> PlayerManager::players = {};
int PlayerManager::currentPlayerIndex = 0;

PlayerManager::PlayerManager() = default;
PlayerManager::~PlayerManager() = default;

void PlayerManager::createPlayers() {
    players.push_back(Player(WHITE));
    players.push_back(Player(BLACK));
}

void PlayerManager::switchPlayers() {
    currentPlayerIndex = (currentPlayerIndex + 1) % 2;
}

Player * PlayerManager::getCurrentPlayer() {
    return &players[currentPlayerIndex];
}

Player * PlayerManager::getOpponent() {
    return &players[(currentPlayerIndex + 1) % 2];
}
