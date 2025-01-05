//
// Created by Noah Schwenold on 1/3/2025.
//

#ifndef SQUARE_H
#define SQUARE_H
#include <memory>
#include <vector>
#include "../misc/Color.h"
#include "../pieces/MoveUpdater.h"

class MoveUpdater;
class Piece;

class Square {
public:
    Square(int row, int col, Piece* piece, Color color);

    [[nodiscard]] int getRow() const;
    [[nodiscard]] int getColumn() const;
    [[nodiscard]] Color getColor() const;
    [[nodiscard]] Piece* getPiece() const;
    [[nodiscard]] bool hasPiece() const;
    [[nodiscard]] std::vector<Piece*>* getAttackingPieces();
    [[nodiscard]] std::vector<Piece*>* getAttackingPieces(Color color);
    void setPiece(Piece* piece);
    void removePiece();

    void addAttackingPiece(Piece* piece);
    void removeAttackingPiece(Piece* piece);

    [[nodiscard]] MoveUpdater* getUpdater() const;

    void print() const;

private:
    int row;
    int col;
    Color color;
    Piece* piece;
    MoveUpdater* updater;
    std::vector<Piece*> whiteAttackingPieces;
    std::vector<Piece*> blackAttackingPieces;
};

#endif // SQUARE_H

