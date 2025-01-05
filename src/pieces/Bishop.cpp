//
// Created by Noah Schwenold on 1/5/2025.
//

#include "Bishop.h"

#include "../board/BoardManager.h"

Bishop::~Bishop() = default;

Bishop::Bishop(PieceType type, Color color, Square* square) : type(type), color(color), square(square) {
    square->setPiece(this);
}

Color Bishop::getColor() {
    return color;
}

PieceType Bishop::getType() {
    return type;
}

Square* Bishop::getSquare() {
    return square;
}

bool Bishop::hasMoved() {
    return moved;
}

void Bishop::move(Square* square, bool realMove) {
    this->square->removePiece();
    for (const auto attacking_piece : *this->square->getAttackingPieces()) {
        attacking_piece->updateLegalMoves(true);
    }

    square->setPiece(this);
    this->square = square;
    for (const auto attacking_piece : *square->getAttackingPieces()) {
        attacking_piece->updateLegalMoves(true);
    }

    updateLegalMoves(true);

    moved = realMove ? true : moved;
}

void Bishop::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* Bishop::getLegalMoves() {
    return &legalMoves;
}

void Bishop::updateLegalMoves(bool checkForCheck) {
    for (auto legal_move : legalMoves) {
        legal_move->removeAttackingPiece(this);
    }

    // Get all diagonal squares in four directions
    std::vector<Square*> upRightSquares = BoardManager::getDiagonalSquares(this, true, true);
    std::vector<Square*> upLeftSquares = BoardManager::getDiagonalSquares(this, true, false);
    std::vector<Square*> downRightSquares = BoardManager::getDiagonalSquares(this, false, true);
    std::vector<Square*> downLeftSquares = BoardManager::getDiagonalSquares(this, false, false);

    // Traverse squares until a piece is found
    std::vector<Square*> upRightMoves = BoardManager::traverseSquaresUntilPiece(upRightSquares, color);
    std::vector<Square*> upLeftMoves = BoardManager::traverseSquaresUntilPiece(upLeftSquares, color);
    std::vector<Square*> downRightMoves = BoardManager::traverseSquaresUntilPiece(downRightSquares, color);
    std::vector<Square*> downLeftMoves = BoardManager::traverseSquaresUntilPiece(downLeftSquares, color);

    // Combine all potential moves
    std::vector<Square*> potentialMoves;
    potentialMoves.insert(potentialMoves.end(), upRightMoves.begin(), upRightMoves.end());
    potentialMoves.insert(potentialMoves.end(), upLeftMoves.begin(), upLeftMoves.end());
    potentialMoves.insert(potentialMoves.end(), downRightMoves.begin(), downRightMoves.end());
    potentialMoves.insert(potentialMoves.end(), downLeftMoves.begin(), downLeftMoves.end());

    setLegalMoves(BoardManager::validateMoves(this, potentialMoves, color, checkForCheck));

    for (auto legal_move : legalMoves) {
        legal_move->addAttackingPiece(this);
    }
}
