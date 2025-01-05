//
// Created by Noah Schwenold on 1/4/2025.
//

#include "King.h"

#include "../board/BoardManager.h"

King::~King() = default;

King::King(const PieceType type, const Color color, Square* square) : type(type), color(color), square(square) {
    square->setPiece(this);
}

Color King::getColor() {
    return color;
}

PieceType King::getType() {
    return type;
}

Square * King::getSquare() {
    return square;
}

bool King::hasMoved() {
    return moved;
}

void King::move(Square* square, const bool realMove) {
    this->square->removePiece();

    Square* oldSquare = this->square;
    square->setPiece(this);
    this->square = square;

    MoveUpdater::updateAll(oldSquare, this);

    updateLegalMoves(false);

    moved = realMove ? true : moved;
}

void King::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* King::getLegalMoves() {

    return &legalMoves;
}

void King::updateLegalMoves(bool checkForCheck) {
    for (auto legal_move : legalMoves) {
        legal_move->removeAttackingPiece(this);
    }

    std::vector<Pair> moves = {{1, 0}, {1, 1}, {1, -1}, {0, 1}, {0, -1}, {-1, 0}, {-1, 1}, {-1, -1}};

    auto potentialMoves = BoardManager::getSquaresByOffset(this, moves);
    setLegalMoves(BoardManager::validateMoves(this, potentialMoves, color, checkForCheck));

    for (const auto move : potentialMoves) {
        move->addAttackingPiece(this);
    }

    MoveUpdater::update(this, &potentialMoves);
}

std::vector<MoveUpdater *> * King::getUpdates() {
    return &updates;
}

void King::setUpdate(std::vector<MoveUpdater*>& updates) {
    this->updates = std::move(updates);
}

void King::updatePinnedPairs() {
    for (int i = 0; i < 4; ++i) {
        std::vector<Square*> squares = BoardManager::getDiagonalSquares(this, i < 2, i % 2);
        std::pair<Piece*, Piece*> pinnedPair = BoardManager::checkForPinedPiece(squares, color);

        if (pinnedPair.first != nullptr) {
            pinnedPairs.insert(pinnedPair);
        }
    }

    for (int i = 0; i < 4; ++i) {
        std::vector<Square*> squares = BoardManager::getStraightSquares(this, i < 2, i % 2);
        std::pair<Piece*, Piece*> pinnedPair = BoardManager::checkForPinedPiece(squares, color);

        if (pinnedPair.first != nullptr) {
            pinnedPairs.insert(pinnedPair);
        }
    }
}

std::unordered_map<Piece *, Piece *> * King::getPinnedPairs() {
    return &pinnedPairs;
}

bool King::isPinned(Piece* piece) const {
    return pinnedPairs.contains(piece);
}

Piece * King::getPinningPiece(Piece *piece) const {
    return pinnedPairs.at(piece);
}
