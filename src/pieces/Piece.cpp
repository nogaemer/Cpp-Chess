//
// Created by Noah Schwenold on 1/5/2025.
//

#include "Piece.h"
#include "../board/BoardManager.h"

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

