//
// Created by Noah Schwenold on 1/8/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "../misc/Color.h"


class Player {
public:
    explicit Player(Color color);
    ~Player();

    Color getColor();
private:
    Color color;
};


#endif //PLAYER_H
