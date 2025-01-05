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

    auto squares = BoardManager::getSquaresByOffset(this, moves);
    setLegalMoves(BoardManager::validateMoves(this, squares, color, checkForCheck));

    for (auto legal_move : legalMoves) {
        legal_move->addAttackingPiece(this);
    }
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
