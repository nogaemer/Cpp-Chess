//
// Created by Noah Schwenold on 1/3/2025.
//

#include "../pieces/Knight.h"

#include "../board/BoardManager.h"

Knight::~Knight() = default;

Knight::Knight(const PieceType type, const Color color, Square* square) : type(type), color(color), square(square) {
    square->setPiece(this);
}

Color Knight::getColor() {
    return color;
}

PieceType Knight::getType() {
    return type;
}

Square * Knight::getSquare() {
    return square;
}

bool Knight::hasMoved() {
    return moved;
}

void Knight::move(Square* square, const bool realMove) {
    this->square->removePiece();

    Square* oldSquare = this->square;
    square->setPiece(this);
    this->square = square;

    MoveUpdater::updateAll(oldSquare, this);

    updateLegalMoves(true);

    moved = realMove ? true : moved;
}

void Knight::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* Knight::getLegalMoves() {

    return &legalMoves;
}

void Knight::updateLegalMoves(bool checkForCheck) {
    for (auto legal_move : legalMoves) {
        legal_move->removeAttackingPiece(this);
    }

    std::vector<Pair> moves = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    auto potentialMoves = BoardManager::getSquaresByOffset(this, moves);
    setLegalMoves(BoardManager::validateMoves(this, potentialMoves, color, checkForCheck));

    for (const auto move : potentialMoves) {
        move->addAttackingPiece(this);
    }
    MoveUpdater::update(this, &potentialMoves);
}

std::vector<MoveUpdater *> * Knight::getUpdates() {
    return &updates;
}

void Knight::setUpdate(std::vector<MoveUpdater*>& updates) {
    this->updates = std::move(updates);
}