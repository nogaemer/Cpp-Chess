//
// Created by Noah Schwenold on 1/5/2025.
//

#ifndef BISHOP_H
#define BISHOP_H

#include <string>

#include "Piece.h"

class Bishop final : public Piece {
public:
    ~Bishop() override;
    Bishop(PieceType type, Color color, Square* square);

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

    std::string shortName() override {return "B";}

private:
    PieceType type;
    Color color;
    Square* square;

    bool moved = false;

    std::vector<Square*> legalMoves;
    std::vector<MoveUpdater*> updates;
};

#endif //BISHOP_H