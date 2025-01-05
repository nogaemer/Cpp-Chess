//
// Created by Noah Schwenold on 1/4/2025.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <windows.h>

#include "Square.h"
#include "../misc/Pair.h"
#include "../pieces/Piece.h"

struct RGB;

class Display {
public:
    void start();
private:

    int xCursorPosition = 5;
    int yCursorPosition = 6;
    bool pieceSelected = false;
    int* selectedMove = &xCursorPosition;
    Piece* selectedPiece = nullptr;

    int maxXCursorPosition = 8;
    int maxYCursorPosition = 8;

    void drawBoard() const;

    void gotoXY(short x, short y);

    static void setBackgroundColor(RGB color);
    static void setForegroundColor(RGB color);
    static void resetColor();

    static void disableCursor();
};

#endif //DISPLAY_H
