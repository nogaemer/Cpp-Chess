//
// Created by Noah Schwenold on 1/5/2025.
//

#include "MoveUpdater.h"

#include <algorithm>
#include <set>

#include "Piece.h"
#include "../board/Square.h"

void MoveUpdater::registerPiece(Piece *piece) {
    pieces.push_back(piece);
}

void MoveUpdater::unregisterPiece(Piece *piece) {
    const auto it = std::ranges::find(pieces, piece);
    if (it != pieces.end()) {
        pieces.erase(it);
    }
}

std::vector<Piece *> * MoveUpdater::getRegisteredPieces() {
    return &pieces;
}

void MoveUpdater::removePieceFromUpdates(Piece *piece) {
    for (MoveUpdater* update : *piece->getUpdates()) {
        update->unregisterPiece(piece);
    }
    piece->getUpdates()->clear();
}

void MoveUpdater::update(Piece *piece, std::vector<Square *> *updates) {
    for (MoveUpdater* update : *piece->getUpdates()) {
        update->unregisterPiece(piece);
    }
    piece->getUpdates()->clear();

    for (Square* update : *updates) {
        MoveUpdater* updater = update->getUpdater();
        updater->registerPiece(piece);
        piece->getUpdates()->push_back(updater);
    }
}

void MoveUpdater::updateAll(Square *oldSquare, Piece *piece) {
    std::set<Piece*> uniqueAttackingPieces;

    // Insert elements from the first vector
    for (const auto attacking_piece : *piece->getSquare()->getUpdater()->getRegisteredPieces()) {
        uniqueAttackingPieces.insert(attacking_piece);
    }

    // Insert elements from the second vector
    for (const auto attacking_piece : *oldSquare->getUpdater()->getRegisteredPieces()) {
        uniqueAttackingPieces.insert(attacking_piece);
    }

    uniqueAttackingPieces.erase(piece);

    for (const auto attacking_piece : uniqueAttackingPieces) {
        attacking_piece->updateLegalMoves(true);
    }
}
