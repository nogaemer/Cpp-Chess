#include <iostream>

#include "board/BoardManager.h"
#include "board/Display.h"

int main() {
    try {
        BoardManager::createChessBoard();

        Display display;
        display.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
    }

    return 0;
}
