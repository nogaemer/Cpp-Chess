//
// Created by Noah Schwenold on 1/5/2025.
//

#include "Piece.h"
#include "../board/BoardManager.h"

void Piece::move(Square* square, bool realMove) {

    this->getSquare()->removePiece();
    if (square->getPiece() != nullptr) {
        square->getPiece()->deletePiece();
    }

    Square* oldSquare = this->getSquare();
    square->setPiece(this);
    this->setSquare(square);

    for (auto legal_move : *getLegalMoves()) {
        legal_move->removeAttackingPiece(this);
    }

    this->updateLegalMoves(true);

    MoveUpdater::updateAll(oldSquare, this);

    setMoved(realMove ? true : hasMoved());
}

void Piece::deletePiece() {
    for (auto square : *getLegalMoves()) {
        square->removeAttackingPiece(this);
    }
    for (auto update : *getUpdates()) {
        update->unregisterPiece(this);
    }
    getSquare()->removePiece();

    BoardManager::removePiece(this);
}

