//
// Created by Noah Schwenold  on 1/3/2025.
//

#include "Square.h"
#include <iostream>
#include <algorithm>

#include "BoardManager.h"
#include "../pieces/King.h"
#include "../pieces/Piece.h"

Square::Square(const int row, const int col, Piece* piece, const Color color
    ): row(row), col(col), color(color), piece(piece), updater(new MoveUpdater) {}

int Square::getRow() const {
    return row;
}

int Square::getColumn() const {
    return col;
}

Color Square::getColor() const {
    return color;
}

Piece* Square::getPiece() const {
    return piece;
}

void Square::setPiece(Piece* piece) {
    this->piece = piece;
}

bool Square::hasPiece() const {
    return piece != nullptr;
}

void Square::removePiece() {
    piece = nullptr;
}

MoveUpdater * Square::getUpdater() const {
    return updater;
}

std::vector<Piece *> * Square::getAttackingPieces() {
    auto* combinedAttackingPieces = new std::vector<Piece*>();
    combinedAttackingPieces->insert(combinedAttackingPieces->end(), whiteAttackingPieces.begin(), whiteAttackingPieces.end());
    combinedAttackingPieces->insert(combinedAttackingPieces->end(), blackAttackingPieces.begin(), blackAttackingPieces.end());
    return combinedAttackingPieces;
}

std::vector<Piece *> * Square::getAttackingPieces(const Color color) {
    if (color == WHITE) {
        return &whiteAttackingPieces;
    } else {
        return &blackAttackingPieces;
    }
}

void Square::addAttackingPiece(Piece* piece) {
    std::vector<Piece*>* attackingPieces = piece->getColor() == WHITE ? &whiteAttackingPieces : &blackAttackingPieces;

    if (std::ranges::find(*attackingPieces, piece) != attackingPieces->end()) {
        throw std::runtime_error("Piece already attacking square");
    }

    attackingPieces->push_back(piece);

    if (this->piece != nullptr && this->piece->getType() == PieceType::KING) {

        std::vector<Piece*> coloredPieces;
        if (piece->getColor() != WHITE) {
            coloredPieces = BoardManager::getWhitePieces();
            BoardManager::getKing(WHITE)->updateLegalMoves(false);
        } else {
            coloredPieces = BoardManager::getBlackPieces();
            BoardManager::getKing(WHITE)->updateLegalMoves(false);
        }
        for (const auto coloredPiece : coloredPieces) {
            coloredPiece->updateLegalMoves(true);
        }
    }
}

void Square::removeAttackingPiece(Piece* piece) {
    std::vector<Piece*>* attackingPieces = piece->getColor() == WHITE ? &whiteAttackingPieces : &blackAttackingPieces;

    auto it = std::ranges::find(*attackingPieces, piece);
    if (it != attackingPieces->end()) {
        attackingPieces->erase(it);
    }

    if (this->piece != nullptr && this->piece->getType() == PieceType::KING) {

        std::vector<Piece*> coloredPieces;
        if (piece->getColor() != WHITE) {
            coloredPieces = BoardManager::getWhitePieces();
            BoardManager::getKing(WHITE)->updateLegalMoves(false);
        } else {
            coloredPieces = BoardManager::getBlackPieces();
            BoardManager::getKing(BLACK)->updateLegalMoves(false);
        }
        for (const auto coloredPiece : coloredPieces) {
            coloredPiece->updateLegalMoves(true);
        }
    }
}


void Square::print() const {
    if (piece != nullptr) {
        std::cout << "Class: Square" << std::endl;
        std::cout << "  Row: " << row << std::endl;
        std::cout << "  Col: " << col << std::endl;
        std::cout << "  Piece: " << piece << std::endl;
    } else {
        std::cout << " ";
    }
}

