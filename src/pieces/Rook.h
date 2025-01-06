//
// Created by Noah Schwenold on 1/4/2025.
//

#ifndef ROOK_H
#define ROOK_H

#include <string>

#include "Piece.h"

class Rook final : public Piece {
public:
    ~Rook() override;
    Rook(PieceType type, Color color, Square* square);

    Color getColor() override;
    PieceType getType() override;
    Square* getSquare() override;
    void setSquare(Square* square) override;

    bool hasMoved() override;
    void setMoved(bool moved) override;

    void setLegalMoves(std::vector<Square *> moves) override;
    std::vector<Square *>* getLegalMoves() override;
    void updateLegalMoves(bool checkForCheck) override;

    std::vector<MoveUpdater*>* getUpdates() override;
    void setUpdate(std::vector<MoveUpdater*>& updates) override;

    std::string shortName() override {return "R";}

private:
    PieceType type;
    Color color;
    Square* square;

    bool moved = false;

    std::vector<Square*> legalMoves;
    std::vector<MoveUpdater*> updates;
};

#endif //ROOK_H
