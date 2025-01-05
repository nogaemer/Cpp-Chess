//
// Created by Noah Schwenold on 1/5/2025.
//

// Pawn.h

#ifndef PAWN_H
#define PAWN_H

#include <string>

#include "Piece.h"

class Pawn final : public Piece {
public:
    ~Pawn() override;
    Pawn(PieceType type, Color color, Square* square);

    Color getColor() override;
    PieceType getType() override;
    Square* getSquare() override;

    bool hasMoved() override;
    void move(Square* square, bool realMove) override;

    void setLegalMoves(std::vector<Square *> moves) override;
    std::vector<Square *>* getLegalMoves() override;
    void updateLegalMoves(bool checkForCheck) override;

    std::string shortName() override {return "P";}

private:
    PieceType type;
    Color color;
    Square* square;

    bool moved = false;

    std::vector<Square*> legalMoves;
};

#endif //PAWN_H