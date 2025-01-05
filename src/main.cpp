#include <iostream>

#include "board/BoardManager.h"
#include "board/Display.h"

int main() {

    BoardManager::createChessBoard();

    Display display;
    display.start();

    return 0;
}
