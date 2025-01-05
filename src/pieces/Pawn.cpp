//
// Created by Noah Schwenold on 1/5/2025.
//

#include "Pawn.h"
#include "../board/BoardManager.h"

Pawn::~Pawn() = default;

Pawn::Pawn(PieceType type, Color color, Square* square) : type(type), color(color), square(square) {
    square->setPiece(this);
}

Color Pawn::getColor() {
    return color;
}

PieceType Pawn::getType() {
    return type;
}

Square* Pawn::getSquare() {
    return square;
}

bool Pawn::hasMoved() {
    return moved;
}

void Pawn::move(Square* square, bool realMove) {
    this->square->removePiece();

    Square* oldSquare = this->square;
    square->setPiece(this);
    this->square = square;

    MoveUpdater::updateAll(oldSquare, this);

    updateLegalMoves(true);

    moved = realMove ? true : moved;
}

void Pawn::setLegalMoves(std::vector<Square *> moves) {
    legalMoves = std::move(moves);
}

std::vector<Square *>* Pawn::getLegalMoves() {
    return &legalMoves;
}

void Pawn::updateLegalMoves(bool checkForCheck) {
    for (auto legal_move : legalMoves) {
        legal_move->removeAttackingPiece(this);
    }

    std::vector<Square*> potentialMoves;

    // Get the forward move
    Pair forwardOffset = {color == WHITE ? 1 : -1, 0};
    Pair forwardPosition = BoardManager::getPositionByOffset({square->getRow(), square->getColumn()}, forwardOffset);
    if (BoardManager::isOnBoard(forwardPosition)) {
        Square* forwardSquare = BoardManager::getSquare(forwardPosition);
        forwardSquare->addAttackingPiece(this);
        if (!forwardSquare->hasPiece()) {
            potentialMoves.push_back(forwardSquare);
        }
    }

    // Get the initial double move
    if (!moved) {
        Pair doubleForwardOffset = {color == WHITE ? 2 : -2, 0};
        Pair doubleForwardPosition = BoardManager::getPositionByOffset({square->getRow(), square->getColumn()}, doubleForwardOffset);
        if (BoardManager::isOnBoard(doubleForwardPosition)) {
            Square* doubleForwardSquare = BoardManager::getSquare(doubleForwardPosition);
            doubleForwardSquare->addAttackingPiece(this);
            if (!doubleForwardSquare->hasPiece() && !potentialMoves.empty()) {
                potentialMoves.push_back(doubleForwardSquare);
            }
        }
    }

    // Get the capture moves
    std::vector<Pair> captureOffsets = {{color == WHITE ? 1 : -1, -1}, {color == WHITE ? 1 : -1, 1}};
    for (const Pair& offset : captureOffsets) {
        Pair capturePosition = BoardManager::getPositionByOffset({square->getRow(), square->getColumn()}, offset);
        if (BoardManager::isOnBoard(capturePosition)) {
            Square* captureSquare = BoardManager::getSquare(capturePosition);
            captureSquare->addAttackingPiece(this);
            if (captureSquare->hasPiece() && captureSquare->getPiece()->getColor() != color) {
                potentialMoves.push_back(captureSquare);
            }
        }
    }

    setLegalMoves(BoardManager::validateMoves(this, potentialMoves, color, checkForCheck));

    for (auto legal_move : legalMoves) {
        legal_move->addAttackingPiece(this);
    }
    MoveUpdater::update(this, &potentialMoves);
}

std::vector<MoveUpdater *> * Pawn::getUpdates() {
    return &updates;
}

void Pawn::setUpdate(std::vector<MoveUpdater*>& updates) {
    this->updates = std::move(updates);
}