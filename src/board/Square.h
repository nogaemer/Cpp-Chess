//
// Created by Noah Schwenold on 1/3/2025.
//

#ifndef SQUARE_H
#define SQUARE_H
#include <vector>

#include "../misc/Color.h"


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
    [[nodiscard]] std::vector<Piece*>* getSecondaryAttackingPieces();
    void setPiece(Piece* piece);
    void removePiece();

    void addAttackingPiece(Piece* piece);
    void removeAttackingPiece(const Piece* piece);

    void addSecondaryAttackingPiece(Piece* piece);
    void removeSecondaryAttackingPiece(const Piece* piece);

    void print() const;

private:
    int row;
    int col;
    Color color;
    Piece* piece;
    std::vector<Piece*> attackingPieces;
    std::vector<Piece*> secondaryAttackingPieces;
};

#endif // SQUARE_H

