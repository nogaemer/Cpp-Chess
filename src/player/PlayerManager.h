//
// Created by Noah Schwenold on 1/8/2025.
//

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H
#include <vector>

#include "Player.h"

class PlayerManager {
public:
    PlayerManager();
    ~PlayerManager();

    static void createPlayers();
    static void switchPlayers();
    static Player* getCurrentPlayer();
    static Player* getOpponent();
private:
    static std::vector<Player> players;
    static int currentPlayerIndex;
};

#endif //PLAYERMANAGER_H
