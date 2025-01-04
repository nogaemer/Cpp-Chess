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
    square->setPiece(this);
    this->square = square;

    moved = realMove ? true : moved;
}

void Knight::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* Knight::getLegalMoves() {

    return &legalMoves;
}

void Knight::updateLegalMoves(bool checkForCheck) {
    std::vector<Pair> moves = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    auto squares = BoardManager::getSquaresByOffset(this, moves);
    setLegalMoves(BoardManager::validateMoves(this, squares, color, checkForCheck));
}