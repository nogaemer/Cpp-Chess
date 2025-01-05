//
// Created by Noah Schwenold  on 1/3/2025.
//

#include "Square.h"
#include <iostream>
#include <algorithm>

#include "../pieces/Piece.h"

Square::Square(const int row, const int col, Piece* piece, const Color color
    ): row(row), col(col), color(color), piece(piece) {}

int Square::getRow() const {
    return row;
}

int Square::getColumn() const {
    return col;
}

Color Square::getColor() const {
    return color;
}

Piece* Square::getPiece() const {
    return piece;
}

void Square::setPiece(Piece* piece) {
    this->piece = piece;
}

bool Square::hasPiece() const {
    return piece != nullptr;
}

void Square::removePiece() {
    piece = nullptr;
}

MoveUpdater * Square::getUpdater() const {
    return updater;
}

std::vector<Piece *> * Square::getAttackingPieces() {
    return &attackingPieces;
}

void Square::addAttackingPiece(Piece* piece) {
    if (std::ranges::find(attackingPieces, piece) != attackingPieces.end()) {
        throw std::runtime_error("Piece already attacking square");
    }
    attackingPieces.push_back(piece);
}

void Square::removeAttackingPiece(const Piece* piece) {
    auto it = std::ranges::find(attackingPieces, piece);
    if (it != attackingPieces.end()) {
        attackingPieces.erase(it);
    }
}


void Square::print() const {
    if (piece != nullptr) {
        std::cout << "Class: Square" << std::endl;
        std::cout << "  Row: " << row << std::endl;
        std::cout << "  Col: " << col << std::endl;
        std::cout << "  Piece: " << piece << std::endl;
    } else {
        std::cout << " ";
    }
}

