//
// Created by Noah Schwenold on 1/4/2025.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <windows.h>

#include "Square.h"
#include "../misc/Pair.h"

struct RGB;
class Piece;

class Display {
public:
    void start();
private:

    int xCursorPosition = 0;
    int yCursorPosition = 0;

    int maxXCursorPosition = 8;
    int maxYCursorPosition = 8;

    void drawBoard(Pair selectedSquare);
    void printSquare(Square* square, Pair position,
        bool left, bool right, bool up, bool down,
        RGB bg, RGB fg, RGB highlight);

    void gotoXY(short x, short y);

    void setBackgroundColor(RGB color);
    void setForegroundColor(RGB color);
    void resetColor();

    void disableCursor();
};

#endif //DISPLAY_H
