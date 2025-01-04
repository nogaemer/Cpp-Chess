#include <iostream>

#include "board/BoardManager.h"
#include "board/Display.h"

int main() {

    BoardManager::createChessBoard();

    Display display;
    display.start();

    std::cout << "Press any key to enter..." << std::endl;
    std::cin.get();

    return 0;
}
