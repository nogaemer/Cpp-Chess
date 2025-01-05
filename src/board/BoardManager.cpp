//
// Created by Noah Schwenold on 1/3/2025.
//

#include "BoardManager.h"

#include <cmath>
#include <iostream>

#include "../pieces/Bishop.h"
#include "../pieces/King.h"
#include "../pieces/Knight.h"
#include "../pieces/Pawn.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"

std::unordered_map<Pair, std::unique_ptr<Square>> BoardManager::board {};
std::vector<std::unique_ptr<Piece>> BoardManager::whitePieces {};
std::vector<std::unique_ptr<Piece>> BoardManager::blackPieces {};
King* BoardManager::whiteKing = nullptr;
King* BoardManager::blackKing = nullptr;

BoardManager::BoardManager() = default;

void BoardManager::createChessBoard() {
    // Create the squares
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            Color color = (i + j) % 2 == 0 ? WHITE : BLACK;
            Pair pair = {i, j};

            board.insert(std::make_pair(pair, std::make_unique<Square>(i, j, nullptr, color)));
        }
    }

    // Create the pieces
    whiteKing = new King(PieceType::KING, WHITE, getSquare({0, 4}));
    blackKing = new King(PieceType::KING, BLACK, getSquare({7, 4}));

    whitePieces.push_back(std::make_unique<Rook>(PieceType::ROOK, WHITE, getSquare({0, 0})));
    whitePieces.push_back(std::make_unique<Knight>(PieceType::KNIGHT, WHITE, getSquare({0, 1})));
    whitePieces.push_back(std::make_unique<Bishop>(PieceType::BISHOP, WHITE, getSquare({0, 2})));
    whitePieces.push_back(std::make_unique<Bishop>(PieceType::BISHOP, WHITE, getSquare({0, 5})));
    whitePieces.push_back(std::make_unique<Knight>(PieceType::KNIGHT, WHITE, getSquare({0, 6})));
    whitePieces.push_back(std::make_unique<Rook>(PieceType::ROOK, WHITE, getSquare({0, 7})));
    whitePieces.push_back(std::make_unique<Queen>(PieceType::QUEEN, WHITE, getSquare({0, 3})));
    for (int i = 0; i < 8; i++) {
        whitePieces.push_back(std::make_unique<Pawn>(PieceType::PAWN, WHITE, getSquare({1, i})));
    }

    blackPieces.push_back(std::make_unique<Rook>(PieceType::ROOK, BLACK, getSquare({7, 0})));
    blackPieces.push_back(std::make_unique<Knight>(PieceType::KNIGHT, BLACK, getSquare({7, 1})));
    blackPieces.push_back(std::make_unique<Bishop>(PieceType::BISHOP, BLACK, getSquare({7, 2})));
    blackPieces.push_back(std::make_unique<Bishop>(PieceType::BISHOP, BLACK, getSquare({7, 5})));
    blackPieces.push_back(std::make_unique<Knight>(PieceType::KNIGHT, BLACK, getSquare({7, 6})));
    blackPieces.push_back(std::make_unique<Rook>(PieceType::ROOK, BLACK, getSquare({7, 7})));
    blackPieces.push_back(std::make_unique<Queen>(PieceType::QUEEN, BLACK, getSquare({7, 3})));
    for (int i = 0; i < 8; i++) {
        blackPieces.push_back(std::make_unique<Pawn>(PieceType::PAWN, BLACK, getSquare({6, i})));
    }

    whiteKing->updatePinnedPairs();
    blackKing->updatePinnedPairs();

    for (const auto & white_piece : whitePieces) {
        white_piece->updateLegalMoves(true);
    }
    for (const auto & white_piece : blackPieces) {
        white_piece->updateLegalMoves(true);
    }

    blackKing->updateLegalMoves(false);

    whiteKing->updateLegalMoves(false);
}

Square * BoardManager::getSquare(const Pair &pair) {
    return board.at(pair).get();
}

Piece * BoardManager::getPiece(const PieceType type, const Color color) {
    if (color == WHITE) {
        for (const auto& piece : whitePieces) {
            if (piece->getType() == type) {
                return piece.get();
            }
        }
    } else {
        for (const auto& piece : blackPieces) {
            if (piece->getType() == type) {
                return piece.get();
            }
        }
    }
    return nullptr;
}

Pair BoardManager::getPositionByOffset(const Pair &pair, const Pair &offset) {
    return {pair.row + offset.row, pair.column + offset.column};
}

std::vector<Square*> BoardManager::getSquaresByOffset(Piece* piece, const std::vector<Pair> &offsets) {
    std::vector<Square*> squares {};

    for (auto offset : offsets) {
        Pair position = getPositionByOffset(
            {piece->getSquare()->getRow(), piece->getSquare()->getColumn()}, offset);

        if (!isOnBoard(position)) continue;

        // Check if the square is occupied by a piece of the same color
        squares.push_back(getSquare(position));
    }
    return squares;
}

bool BoardManager::isOnBoard(const Pair &pair) {
    return (pair.row >= 0 && pair.row < 8 && pair.column >= 0 && pair.column < 8);
}

King* BoardManager::getKing(const Color color) {
    return color == WHITE ? whiteKing : blackKing;
}

std::vector<Square*> BoardManager::validateMoves(Piece* piece, std::vector<Square*> &squares, Color color, bool checkForCheck) {
    std::vector<Square*> movableSquares {};
    King* king = getKing(color);

    // If the king is in check, the only legal moves are those that move the king
    if (!king->getSquare()->getAttackingPieces()->empty()) return movableSquares;

    for (auto& square : squares) {
        //Check if the square is occupied by a piece of the same color
        if (square == nullptr) continue;
        if (square->hasPiece() && square->getPiece()->getColor() == color) continue;


        // If the king is not in check, we need to check if the move puts the king in check
        if (king->isPinned(piece) && checkForCheck) {
            if (isMovePuttingKingInCheck(piece, square, king)) continue;
        }

        movableSquares.push_back(square);
    }
    return movableSquares;
}

bool BoardManager::isMovePuttingKingInCheck(Piece* piece, Square* square, King* king) {
    // Save the original moves of the pinning piece
    // Square* originalSquare = piece->getSquare();
    // std::vector<Square *> originalMoves = {};
    //
    // piece->move(square, false);
    //
    // // Get the pinning piece and update its legal moves
    // Piece* attackingPiece = king->getPinningPiece(piece);
    // originalMoves = *attackingPiece->getLegalMoves();
    // Square* attackingSquare = attackingPiece->getSquare();
    // attackingPiece->updateLegalMoves(false);
    //
    // // Check if the king is in check
    // bool isCheck = std::ranges::find(*attackingPiece->getLegalMoves(), king->getSquare()) != attackingPiece->getLegalMoves()->end();
    //
    // // Reset the original moves of the pinning piece
    // piece->move(originalSquare, false);
    // attackingPiece->setLegalMoves(originalMoves);
    // attackingPiece->move(attackingSquare, false);

    Piece *attackingPiece = king->getPinningPiece(piece);

    if (attackingPiece == nullptr) return true;

    if (areCollinear(attackingPiece, square, king) &&
        isAttackingPieceInMiddleOfKingAndPinnedPiece(attackingPiece, square, king)) {
        return false;
    }

    return true;
}

bool BoardManager::isAttackingPieceInMiddleOfKingAndPinnedPiece(Piece* piece, Square* square, King* king) {
    auto dist = [](const Square* s1, const Square* s2) {
        return std::hypot(s2->getRow() - s1->getRow(), s2->getColumn() - s1->getColumn());
    };

    double kingToSquare = dist(king->getSquare(), square);
    double squareToPiece = dist(square, piece->getSquare());
    double kingToPiece = dist(king->getSquare(), piece->getSquare());
    const double epsilon = 1e-9;

    return std::abs(kingToSquare + squareToPiece - kingToPiece) < epsilon ||
           std::abs(kingToSquare + kingToPiece - squareToPiece) < epsilon ||
           std::abs(kingToPiece + squareToPiece - kingToSquare) < epsilon;
}

bool BoardManager::areCollinear(Piece* piece, Square* square, King* king) {
    const int x1 = king->getSquare()->getRow();
    const int y1 = king->getSquare()->getColumn();
    const int x2 = square->getRow();
    const int y2 = square->getColumn();
    const int x3 = piece->getSquare()->getRow();
    const int y3 = piece->getSquare()->getColumn();
    const int area = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    return (area == 0);
}

std::pair<Piece *, Piece *> BoardManager::
checkForPinedPiece(const std::vector<Square *> &squares, const Color color) {
    Piece* pinedPiece = nullptr;
    Piece* pinningPiece = nullptr;

    for (auto& square : squares) {
        if (!square->hasPiece()) continue;

        //check if next piece has a different color
        if (square->getPiece()->getColor() == color) {
            if (pinedPiece == nullptr) {
                pinedPiece = square->getPiece();
                continue;
            }

            return {nullptr, nullptr};
        }

        if (pinedPiece == nullptr) {
            return {nullptr, nullptr};
        }

        pinningPiece = square->getPiece();
        break;

    }

    if (pinedPiece != nullptr && pinningPiece != nullptr) {
        return {pinedPiece, pinningPiece};
    }

    return {nullptr, nullptr};
}

std::vector<Square *> BoardManager::traverseSquaresUntilPiece(const std::vector<Square *> &squares, Color color) {
    std::vector<Square*> traversedSquares;

    for (Square* square : squares) {
        if (square->hasPiece()) {
            if (square->getPiece()->getColor() == color) {
                return traversedSquares;
            }

            traversedSquares.push_back(square);
            return traversedSquares;

        }
        traversedSquares.push_back(square);
    }

    return traversedSquares;
}

std::vector<Square *> BoardManager::getDiagonalSquares(Piece* piece, bool up, bool right) {
    std::vector<Square*> squares {};

    for (int i = 1; i < 8; i++) {
        Pair offset = {up ? i : -i, right ? i : -i};
        Pair position = getPositionByOffset({piece->getSquare()->getRow(), piece->getSquare()->getColumn()}, offset);

        if (!isOnBoard(position)) break;

        Square* square = getSquare(position);
        squares.push_back(square);
    }
    return squares;
}

std::vector<Square *> BoardManager::getStraightSquares(Piece* piece, bool positivDirecion, bool vertical) {
    std::vector<Square*> squares {};

    for (int i = 1; i < 8; i++) {
        Pair offset = {
            vertical ? (positivDirecion ? i : -i) : 0,
            vertical ? 0 : (positivDirecion ? i : -i)
        };
        Pair position = getPositionByOffset({piece->getSquare()->getRow(), piece->getSquare()->getColumn()}, offset);

        if (!isOnBoard(position)) break;

        Square* square = getSquare(position);
        squares.push_back(square);
    }
    return squares;
}



