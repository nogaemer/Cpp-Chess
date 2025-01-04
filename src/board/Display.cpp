//
// Created by Noah Schwenold on 1/4/2025.
//

#include "Display.h"

#include <iostream>
#include <conio.h>

#include "Square.h"
#include "BoardManager.h"

struct RGB {
    int r, g, b;
};

void Display::start() {
    disableCursor();
    drawBoard({0, 0});

    while (true) {
        gotoXY(0, 0);

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                switch (_getch()) {
                    case 75: // Left arrow key
                        if (xCursorPosition > 0) {
                            xCursorPosition--;
                        }
                    break;
                    case 77: // Right arrow key
                        if (xCursorPosition < maxXCursorPosition - 1) {
                            xCursorPosition++;
                        }
                    break;
                    case 72: // Up arrow key
                        if (yCursorPosition > 0) {
                            yCursorPosition--;
                        }
                    break;
                    case 80: // Down arrow key
                        if (yCursorPosition < maxYCursorPosition - 1) {
                            yCursorPosition++;
                        }
                    break;
                    default: break;
                }
                drawBoard({yCursorPosition, xCursorPosition});
            }
        }
    }
}

void Display::drawBoard(Pair selectedSquare) {
    Piece* selectedPiece = BoardManager::getSquare(selectedSquare)->getPiece();

    std::vector<Square*> legalMoveSquares = selectedPiece != nullptr ? *selectedPiece->getLegalMoves() : std::vector<Square*>{};

    gotoXY(0,0);
    disableCursor();

    for (int row = 0; row < 8; row++) {
        gotoXY(0, row * 2 + 3);
        std::cout << 8 - row << " ";
        for (int column = 0; column < 8; column++) {
            Square* square = BoardManager::getSquare({row, column});
            bool isLegalMoveSquare = std::ranges::find(legalMoveSquares, square) != legalMoveSquares.end();

            RGB bg = square->getColor() == WHITE ? RGB{98,88,81} : RGB{46,43,41};
            RGB fg = square->getColor() == WHITE ? RGB{240,240,240} : RGB{224,219,215};
            RGB highlight = RGB{255, 255, 255};

            if (isLegalMoveSquare) {
                bg = RGB{186,255,201};
                fg = RGB{0, 0, 0};
            }

            if (row == selectedSquare.row && column == selectedSquare.column) {
                bg = RGB{128,239,128};
                fg = RGB{0, 0, 0};
            }

            printSquare(square, {row, column},
                true, column == 7, true, row == 7,
                bg, fg, highlight);
        }
    }
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}

void Display::printSquare(Square* square, Pair position,
    bool left, bool right, bool up, bool down,
    RGB bg, RGB fg, RGB highlight) {

    constexpr int xMultiplier = 4;
    constexpr int yMultiplier = 2;
    constexpr int xPadding = 2;
    constexpr int yPadding = 2;

    const auto x = static_cast<short>(position.column * xMultiplier + xPadding);
    auto y = static_cast<short>(position.row * yMultiplier  + yPadding);
    gotoXY(x, y);

    setForegroundColor(highlight);
    if (left && up) std::cout << "+";
    if (up) std::cout << "---";
    if (right && up) std::cout << "+";

    gotoXY(static_cast<short>(position.column * xMultiplier + xPadding),
        static_cast<short>(position.row * yMultiplier) + yPadding + 1);
    if (left) std::cout << "|";

    setBackgroundColor(bg);
    setForegroundColor(fg);

    if (square->hasPiece()) {
        if (square->getPiece()->getColor() == WHITE) {
            std::cout << " W" << square->getPiece()->shortName();
        } else {
            std::cout << " B" << square->getPiece()->shortName();
        }
    } else {
        std::cout << "   ";
    }

    setForegroundColor(highlight);
    if (right) std::cout << "|";

    gotoXY(static_cast<short>(position.column * xMultiplier + xPadding),
        static_cast<short>(position.row * yMultiplier + yPadding + 2));

    resetColor();
    setForegroundColor(highlight);

    if (left && down) std::cout << "+";
    if (down) std::cout << "---";
    if (right && down) std::cout << "+\n";
    std::cout << "\033[0m";
}

void Display::gotoXY(short x, short y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to set background color using hex code
void Display::setBackgroundColor(const RGB color) {
    std::cout << "\033[48;2;" << color.r << ";" << color.g << ";" << color.b << "m";
}

void Display::setForegroundColor(const RGB color) {
    std::cout << "\033[38;2;" << color.r << ";" << color.g << ";" << color.b << "m";
}

void Display::resetColor() {
    std::cout << "\033[0m";
}

void Display::disableCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

