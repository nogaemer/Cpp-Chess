//
// Created by Noah Schwenold on 1/4/2025.
//

#ifndef KING_H
#define KING_H
#include <string>
#include <unordered_map>

#include "Piece.h"

class King final : public Piece {
public:
    ~King() override;
    King(PieceType type, Color color, Square* square);

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

    void updatePinnedPairs();
    std::unordered_map<Piece*, Piece*>* getPinnedPairs();
    bool isPinned(Piece* piece) const;
    Piece* getPinningPiece(Piece* piece) const;

    std::string shortName() override {return "K";}

private:
    PieceType type;
    Color color;
    Square* square;

    bool moved = false;

    std::vector<Square*> legalMoves;
    std::unordered_map<Piece*, Piece*> pinnedPairs;
    std::vector<MoveUpdater*> updates;
};

#endif //KING_H
