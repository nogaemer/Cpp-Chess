//
// Created by Noah Schwenold on 1/8/2025.
//

#include "Player.h"

Player::Player(const Color color): color(color) {
}

Player::~Player() = default;

Color Player::getColor() {
    return color;
}
