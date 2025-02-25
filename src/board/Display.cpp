//
// Created by Noah Schwenold on 1/4/2025.
//

#include "Display.h"

#include <iostream>
#include <conio.h>
#include <sstream>

#include "Square.h"
#include "BoardManager.h"
#include "../pieces/Piece.h"
#include "../player/PlayerManager.h"

struct RGB {
    int r, g, b;

    [[nodiscard]] std::string toForegroundColorCode() const {
        std::ostringstream oss;
        oss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
        return oss.str();
    }

    [[nodiscard]] std::string toBackgroundColorCode() const {
        std::ostringstream oss;
        oss << "\033[48;2;" << r << ";" << g << ";" << b << "m";
        return oss.str();
    }
};

void Display::start() {
    disableCursor();
    drawBoard();

    while (true) {
        gotoXY(0, 0);

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                switch (_getch()) {
                    case 75: // Left arrow key
                        if (xCursorPosition > 0) {
                            xCursorPosition--;
                        } else if (pieceSelected) {
                            xCursorPosition = maxXCursorPosition - 1;
                        }
                    break;
                    case 77: // Right arrow key
                        if (xCursorPosition < maxXCursorPosition - 1) {
                            xCursorPosition++;
                        } else if (pieceSelected) {
                            xCursorPosition = 0;
                        }
                    break;
                    case 72: // Up arrow key
                        if (yCursorPosition > 0) {
                            yCursorPosition--;
                        }
                    break;
                    case 80: // Down arrow key
                        if (pieceSelected) break;

                        if (yCursorPosition < maxYCursorPosition - 1) {
                            yCursorPosition++;
                        }
                    break;
                    default: break;
                }
            } else if (ch == 13) { // Enter key
                if (pieceSelected) {
                    Square* moveToSquare = selectedPiece->getLegalMoves()->at(*selectedMove);
                    selectedPiece->move(moveToSquare, true);
                    PlayerManager::switchPlayers();
                    if (BoardManager::isCheckMate()) drawCheckMateScreen();

                    pieceSelected = false;
                    xCursorPosition = moveToSquare->getColumn();
                    yCursorPosition = moveToSquare->getRow();
                    maxXCursorPosition = 8;
                } else {
                    const Square* square = BoardManager::getSquare({yCursorPosition, xCursorPosition});

                    if (square->hasPiece()) {
                        if (PlayerManager::getCurrentPlayer()->getColor() == square->getPiece()->getColor()) {
                            if (square->hasPiece() && !square->getPiece()->getLegalMoves()->empty()) {
                                pieceSelected = true;
                                selectedPiece = square->getPiece();
                                xCursorPosition = 0;
                                maxXCursorPosition = selectedPiece->getLegalMoves()->size();
                            }
                        }
                    }
                }
            } else if (ch == 27) { // Escape key
                pieceSelected = false;
                xCursorPosition = selectedPiece->getSquare()->getColumn();
                yCursorPosition = selectedPiece->getSquare()->getRow();
                maxXCursorPosition = 8;
            }
            drawBoard();
        }
    }
}

void Display::drawBoard() const {
    Piece* selectedPiece;
    if (pieceSelected) {
        selectedPiece = this->selectedPiece;
    } else {
        selectedPiece = BoardManager::getSquare({yCursorPosition, xCursorPosition})->getPiece();
    }

    std::vector<Square*> legalMoveSquares = std::vector<Square*>{};
    if ( selectedPiece != nullptr && PlayerManager::getCurrentPlayer()->getColor() == selectedPiece->getColor()) {
        legalMoveSquares = *selectedPiece->getLegalMoves();
    }

    RGB bg;
    RGB fg;
    constexpr auto highlight = RGB{255, 255, 255};
    std::string clearColors = "\033[0m";

    std::ostringstream boardStream;
    boardStream << clearColors << "\n\n"; // Reset colors

    for (int row = 0; row < 8; row++) {
        boardStream << "  +---+---+---+---+---+---+---+---+\n";
        boardStream << 8 - row << " |";
        for (int column = 0; column < 8; column++) {
            Square* square = BoardManager::getSquare({row, column});
            bool isLegalMoveSquare = std::ranges::find(legalMoveSquares, square) != legalMoveSquares.end();

            bg = square->getColor() == WHITE ? RGB{98,88,81} : RGB{46,43,41};
            fg = square->getColor() == WHITE ? RGB{240,240,240} : RGB{224,219,215};



            if (pieceSelected) {
                // Highlight selected piece
                if (this->selectedPiece->getSquare() == square) {
                    bg = RGB{0,187,119};
                    fg = RGB{0, 0, 0};
                }

                // Highlight legal moves
                if (isLegalMoveSquare) {
                    bg = RGB{255,116,108};
                    fg = RGB{0, 0, 0};
                }

                // Highlight selected move

                if (this->selectedPiece->getLegalMoves()->at(*selectedMove) == square) {
                    bg = RGB{192,70,87};
                    fg = RGB{0, 0, 0};
                }
            } else {
                if (isLegalMoveSquare) {
                    bg = RGB{255,116,108};
                    fg = RGB{0, 0, 0};
                }

                if (row == yCursorPosition && column == xCursorPosition) {
                    bg = RGB{128,239,128};
                    fg = RGB{0, 0, 0};
                }
            }

            boardStream << bg.toBackgroundColorCode();
            boardStream << fg.toForegroundColorCode();

            if (square->hasPiece()) {
                if (square->getPiece()->getColor() == WHITE) {
                    boardStream << " W" << square->getPiece()->shortName();
                } else {
                    boardStream << " B" << square->getPiece()->shortName();
                }
            } else {
                boardStream << "   ";
            }

            boardStream << clearColors;
            boardStream << highlight.toForegroundColorCode();
            boardStream << "|";
        }
        boardStream << "\n";
    }
    boardStream << "  +---+---+---+---+---+---+---+---+\n";
    boardStream << "    a   b   c   d   e   f   g   h\n";
    std::cout << boardStream.str();
}

void Display::drawCheckMateScreen() {
    std::cout << "Checkmate!";
    std::cout << "Press any key to exit...";
    _getch();
    exit(0);
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

