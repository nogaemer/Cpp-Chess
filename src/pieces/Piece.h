//
// Created by Noah Schwenold  on 1/3/2025.
//

#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <vector>
#include "../board/Square.h"
#include <string>

class MoveUpdater;
class BoardManager;

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

class Piece {
public:
    virtual ~Piece() = default;

    virtual PieceType getType() = 0;
    virtual Color getColor() = 0;
    virtual Square* getSquare() = 0;
    virtual void setSquare(Square* square) = 0;

    virtual bool hasMoved() = 0;
    virtual void setMoved(bool moved) = 0;

    virtual void move(Square* square, bool realMove);
    virtual void deletePiece();

    virtual void setLegalMoves(std::vector<Square*> moves) = 0;
    virtual std::vector<Square *>* getLegalMoves() = 0;
    virtual void updateLegalMoves(bool checkForCheck) = 0;


    virtual std::vector<MoveUpdater*>* getUpdates() = 0;
    virtual void setUpdate(std::vector<MoveUpdater*>& updates) = 0;

    virtual std::string shortName() = 0;

    std::string asString() {
        return "Piece: " + shortName() + " Color: " + (getColor() == Color::WHITE ? "White" : "Black") + " Address: " + std::to_string(reinterpret_cast<uintptr_t>(this));
    }
};

#endif //PIECE_H
