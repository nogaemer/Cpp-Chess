//
// Created by Noah Schwenold on 1/5/2025.
//

#include "Queen.h"
#include "../board/BoardManager.h"

Queen::~Queen() = default;

Queen::Queen(PieceType type, Color color, Square* square) : type(type), color(color), square(square) {
    square->setPiece(this);
}

Color Queen::getColor() {
    return color;
}

PieceType Queen::getType() {
    return type;
}

Square* Queen::getSquare() {
    return square;
}

bool Queen::hasMoved() {
    return moved;
}

void Queen::move(Square* square, bool realMove) {
    this->square->removePiece();
    if (square->getPiece() != nullptr) {
        square->getPiece()->deletePiece();
    }


    Square* oldSquare = this->square;
    square->setPiece(this);
    this->square = square;

    MoveUpdater::updateAll(oldSquare, this);

    updateLegalMoves(true);

    moved = realMove ? true : moved;
}

void Queen::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* Queen::getLegalMoves() {
    return &legalMoves;
}

void Queen::updateLegalMoves(bool checkForCheck) {
    for (auto legal_move : legalMoves) {
        legal_move->removeAttackingPiece(this);
    }

    // Get all straight squares in four directions
    std::vector<Square*> upSquares = BoardManager::getStraightSquares(this, true, true);
    std::vector<Square*> downSquares = BoardManager::getStraightSquares(this, false, true);
    std::vector<Square*> rightSquares = BoardManager::getStraightSquares(this, true, false);
    std::vector<Square*> leftSquares = BoardManager::getStraightSquares(this, false, false);

    // Get all diagonal squares in four directions
    std::vector<Square*> upRightSquares = BoardManager::getDiagonalSquares(this, true, true);
    std::vector<Square*> upLeftSquares = BoardManager::getDiagonalSquares(this, true, false);
    std::vector<Square*> downRightSquares = BoardManager::getDiagonalSquares(this, false, true);
    std::vector<Square*> downLeftSquares = BoardManager::getDiagonalSquares(this, false, false);

    // Traverse squares until a piece is found
    std::vector<Square*> upMoves = BoardManager::traverseSquaresUntilPiece(upSquares, color);
    std::vector<Square*> downMoves = BoardManager::traverseSquaresUntilPiece(downSquares, color);
    std::vector<Square*> rightMoves = BoardManager::traverseSquaresUntilPiece(rightSquares, color);
    std::vector<Square*> leftMoves = BoardManager::traverseSquaresUntilPiece(leftSquares, color);

    std::vector<Square*> upRightMoves = BoardManager::traverseSquaresUntilPiece(upRightSquares, color);
    std::vector<Square*> upLeftMoves = BoardManager::traverseSquaresUntilPiece(upLeftSquares, color);
    std::vector<Square*> downRightMoves = BoardManager::traverseSquaresUntilPiece(downRightSquares, color);
    std::vector<Square*> downLeftMoves = BoardManager::traverseSquaresUntilPiece(downLeftSquares, color);

    // Combine all potential moves
    std::vector<Square*> potentialMoves;
    potentialMoves.insert(potentialMoves.end(), upMoves.begin(), upMoves.end());
    potentialMoves.insert(potentialMoves.end(), downMoves.begin(), downMoves.end());
    potentialMoves.insert(potentialMoves.end(), rightMoves.begin(), rightMoves.end());
    potentialMoves.insert(potentialMoves.end(), leftMoves.begin(), leftMoves.end());
    potentialMoves.insert(potentialMoves.end(), upRightMoves.begin(), upRightMoves.end());
    potentialMoves.insert(potentialMoves.end(), upLeftMoves.begin(), upLeftMoves.end());
    potentialMoves.insert(potentialMoves.end(), downRightMoves.begin(), downRightMoves.end());
    potentialMoves.insert(potentialMoves.end(), downLeftMoves.begin(), downLeftMoves.end());

    setLegalMoves(BoardManager::validateMoves(this, potentialMoves, color, checkForCheck));

    for (auto legal_move : legalMoves) {
        legal_move->addAttackingPiece(this);
    }
    MoveUpdater::update(this, &potentialMoves);
}

std::vector<MoveUpdater *> * Queen::getUpdates() {
    return &updates;
}

void Queen::setUpdate(std::vector<MoveUpdater*>& updates) {
    this->updates = std::move(updates);
}

