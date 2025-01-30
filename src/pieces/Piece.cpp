//
// Created by Noah Schwenold on 1/5/2025.
//

#include "Piece.h"

#include "King.h"
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
    if (this->getType()==PieceType::KING) {
        std::vector<Piece*> attackingPieces = (this->getColor() == WHITE) ? BoardManager::getWhitePieces() : BoardManager::getBlackPieces();
        for (auto attackingPiece : attackingPieces) {
            if (attackingPiece->getType() != PieceType::KING) {
                attackingPiece->updateLegalMoves(true);
            }
        }
        attackingPieces.clear();
    }

    if (this->getType()!=PieceType::KING) {
        Color oppositeColor = (this->getColor() == WHITE) ? BLACK : WHITE;
        dynamic_cast<Piece*>(BoardManager::getKing(this->getColor()))->updateLegalMoves(true);
        dynamic_cast<Piece*>(BoardManager::getKing(oppositeColor))->updateLegalMoves(true);
    }

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

