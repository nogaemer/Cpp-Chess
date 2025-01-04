//
// Created by Noah Schwenold on 1/3/2025.
//

#include "../pieces/Rook.h"

#include "../board/BoardManager.h"

Rook::~Rook() = default;

Rook::Rook(const PieceType type, const Color color, Square* square) : type(type), color(color), square(square) {
    square->setPiece(this);
}

Color Rook::getColor() {
    return color;
}

PieceType Rook::getType() {
    return type;
}

Square * Rook::getSquare() {
    return square;
}

bool Rook::hasMoved() {
    return moved;
}

void Rook::move(Square* square, bool realMove) {
    this->square->removePiece();
    square->setPiece(this);
    this->square = square;

    moved = realMove ? true : moved;
}

void Rook::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* Rook::getLegalMoves() {
    return &legalMoves;
}

void Rook::updateLegalMoves(bool checkForCheck) {
    // Get all straight squares in four directions
    std::vector<Square*> upSquares = BoardManager::getStraightSquares(this, true, true);
    std::vector<Square*> downSquares = BoardManager::getStraightSquares(this, false, true);
    std::vector<Square*> rightSquares = BoardManager::getStraightSquares(this, true, false);
    std::vector<Square*> leftSquares = BoardManager::getStraightSquares(this, false, false);

    // Traverse squares until a piece is found
    std::vector<Square*> upMoves = BoardManager::traverseSquaresUntilPiece(upSquares, color);
    std::vector<Square*> downMoves = BoardManager::traverseSquaresUntilPiece(downSquares, color);
    std::vector<Square*> rightMoves = BoardManager::traverseSquaresUntilPiece(rightSquares, color);
    std::vector<Square*> leftMoves = BoardManager::traverseSquaresUntilPiece(leftSquares, color);

    // Combine all potential moves
    std::vector<Square*> potentialMoves;
    potentialMoves.insert(potentialMoves.end(), upMoves.begin(), upMoves.end());
    potentialMoves.insert(potentialMoves.end(), downMoves.begin(), downMoves.end());
    potentialMoves.insert(potentialMoves.end(), rightMoves.begin(), rightMoves.end());
    potentialMoves.insert(potentialMoves.end(), leftMoves.begin(), leftMoves.end());

    setLegalMoves(BoardManager::validateMoves(this, potentialMoves, color, checkForCheck));
}