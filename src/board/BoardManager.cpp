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
#include "../player/PlayerManager.h"

class Player;
/**
 * @class BoardManager
 * @brief Manages the chess board and pieces.
 */
std::unordered_map<Pair, std::unique_ptr<Square>> BoardManager::board {};
std::vector<std::unique_ptr<Piece>> BoardManager::whitePieces {};
std::vector<std::unique_ptr<Piece>> BoardManager::blackPieces {};
King* BoardManager::whiteKing = nullptr;
King* BoardManager::blackKing = nullptr;

/**
 * @brief Default constructor for BoardManager.
 */
BoardManager::BoardManager() = default;

/**
 * @brief Creates the chess board and initializes the pieces.
 */
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
    whitePieces.push_back(std::make_unique<Rook>(PieceType::ROOK, WHITE, getSquare({6, 5})));
    whitePieces.push_back(std::make_unique<Queen>(PieceType::QUEEN, WHITE, getSquare({0, 3})));
    for (int i = 0; i < 8; i++) {
        if (i == 5) continue;
        whitePieces.push_back(std::make_unique<Pawn>(PieceType::PAWN, WHITE, getSquare({1, i})));
    }

    blackPieces.push_back(std::make_unique<Rook>(PieceType::ROOK, BLACK, getSquare({7, 0})));
    blackPieces.push_back(std::make_unique<Knight>(PieceType::KNIGHT, BLACK, getSquare({7, 1})));
    blackPieces.push_back(std::make_unique<Bishop>(PieceType::BISHOP, BLACK, getSquare({7, 2})));
    blackPieces.push_back(std::make_unique<Knight>(PieceType::KNIGHT, BLACK, getSquare({7, 6})));
    blackPieces.push_back(std::make_unique<Rook>(PieceType::ROOK, BLACK, getSquare({7, 7})));
    blackPieces.push_back(std::make_unique<Queen>(PieceType::QUEEN, BLACK, getSquare({7, 3})));
    for (int i = 0; i < 8; i++) {
        if (i > 4) continue;
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

/**
 * @brief Gets the square at the specified position.
 * @param pair The position of the square.
 * @return Pointer to the square at the specified position.
 */
Square * BoardManager::getSquare(const Pair &pair) {
    return board.at(pair).get();
}

/**
 * @brief Gets the piece of the specified type and color.
 * @param type The type of the piece.
 * @param color The color of the piece.
 * @return Pointer to the piece of the specified type and color.
 */
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



/**
 * @brief Gets the position by applying an offset to a given position.
 * @param pair The original position.
 * @param offset The offset to apply.
 * @return The new position after applying the offset.
 */
Pair BoardManager::getPositionByOffset(const Pair &pair, const Pair &offset) {
    return {pair.row + offset.row, pair.column + offset.column};
}

/**
 * @brief Gets the squares by applying a list of offsets to a piece's position.
 * @param piece The piece whose position is used as the base.
 * @param offsets The list of offsets to apply.
 * @return A vector of pointers to the squares at the new positions.
 */
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

/**
 * @brief Checks if a position is on the board.
 * @param pair The position to check.
 * @return True if the position is on the board, false otherwise.
 */
bool BoardManager::isOnBoard(const Pair &pair) {
    return (pair.row >= 0 && pair.row < 8 && pair.column >= 0 && pair.column < 8);
}

/**
 * @brief Gets the king of the specified color.
 * @param color The color of the king.
 * @return Pointer to the king of the specified color.
 */
King* BoardManager::getKing(const Color color) {
    return color == WHITE ? whiteKing : blackKing;
}

bool BoardManager::isCheckMate() {
    Player* player = PlayerManager::getCurrentPlayer();
    King* king = getKing(player->getColor());
    std::vector<Piece*> pieces = player->getColor() == WHITE ? getWhitePieces() : getBlackPieces();

    if (king->getLegalMoves()->empty()) {
        for (Piece* piece : pieces) {
            if (piece->getLegalMoves()->empty()) continue;
            return false;
        }
        return true;
    }
}

/**
 * @brief Checks if a move removes a check on the king.
 * @param piece The piece to move.
 * @param square The square to move the piece to.
 * @param king The king to check for check removal.
 * @param color The color of the piece.
 * @return True if the move removes the check, false otherwise.
 */
bool BoardManager::doesMoveRemoveCheck(Piece *piece, Square *square, King *king, Color color) {
    std::vector<Piece*>* attackingPieces = king->getSquare()->getAttackingPieces(color == WHITE ? BLACK : WHITE);

    if (piece->getType() == PieceType::KING) {
        if (square->getAttackingPieces()->empty()) {
            return true;
        }
        return false;
    }

    if (attackingPieces->size() > 1) {
        return false;
    }

    if (attackingPieces->at(0)->getSquare() == square) {
        return true;
    }

    if (areCollinear(attackingPieces->at(0), square, king) &&
        isAttackingPieceInMiddleOfKingAndPinnedPiece(attackingPieces->at(0), square, king)) {
        return true;
    }
    return false;
}

/**
 * @brief Validates the possible moves for a piece.
 * @param piece The piece to validate moves for.
 * @param squares The possible squares to move to.
 * @param color The color of the piece.
 * @param checkForCheck Whether to check if the move puts the king in check.
 * @return A vector of pointers to the valid squares the piece can move to.
 */
std::vector<Square*> BoardManager::validateMoves(Piece* piece, std::vector<Square*> &squares, Color color, bool checkForCheck) {
    std::vector<Square*> movableSquares {};
    King* king = getKing(color);

    for (auto& square : squares) {
        // Check if the square is occupied by a piece of the same color
        if (square == nullptr) continue;
        if (square->hasPiece() && square->getPiece()->getColor() == color) continue;

        // Check if the move puts the king in check
        if (piece->getType() == PieceType::KING) {
            if (!square->getAttackingPieces(color == WHITE ? BLACK : WHITE)->empty()) continue;
        }

        // If the king is not in check, we need to check if the move puts the king in check
        if (king->isPinned(piece) && checkForCheck) {
            if (isMovePuttingKingInCheck(piece, square, king)) continue;
        }

        // If the king is in check, we need to check if the move removes the check
        if (!king->getSquare()->getAttackingPieces(color == WHITE ? BLACK : WHITE)->empty() && !doesMoveRemoveCheck(piece, square, king, color)) {
            continue;
        }

        movableSquares.push_back(square);
    }
    return movableSquares;
}

/**
 * @brief Checks if a move puts the king in check.
 * @param piece The piece to move.
 * @param square The square to move the piece to.
 * @param king The king to check for check.
 * @return True if the move puts the king in check, false otherwise.
 */
bool BoardManager::isMovePuttingKingInCheck(Piece* piece, const Square* square, King* king) {
    Piece *attackingPiece = king->getPinningPiece(piece);

    if (attackingPiece == nullptr) return true;

    if (areCollinear(attackingPiece, square, king) &&
        isAttackingPieceInMiddleOfKingAndPinnedPiece(attackingPiece, square, king)) {
        return false;
    }

    return true;
}

/**
 * @brief Checks if an attacking piece is in the middle of the king and a pinned piece.
 * @param piece The attacking piece.
 * @param square The square to move the pinned piece to.
 * @param king The king to check for alignment.
 * @return True if the attacking piece is in the middle, false otherwise.
 */
bool BoardManager::isAttackingPieceInMiddleOfKingAndPinnedPiece(Piece* piece, const Square* square, King* king) {
    auto dist = [](const Square* s1, const Square* s2) {
        return std::hypot(s2->getRow() - s1->getRow(), s2->getColumn() - s1->getColumn());
    };

    const double kingToSquare = dist(king->getSquare(), square);
    const double squareToPiece = dist(square, piece->getSquare());
    const double kingToPiece = dist(king->getSquare(), piece->getSquare());
    constexpr double epsilon = 1e-9;

    return std::abs(kingToSquare + squareToPiece - kingToPiece) < epsilon;
}

/**
 * @brief Checks if three pieces are collinear.
 * @param piece The first piece.
 * @param square The square to move the piece to.
 * @param king The king to check for alignment.
 * @return True if the pieces are collinear, false otherwise.
 */
bool BoardManager::areCollinear(Piece* piece, const Square* square, King* king) {
    const int x1 = king->getSquare()->getRow();
    const int y1 = king->getSquare()->getColumn();
    const int x2 = square->getRow();
    const int y2 = square->getColumn();
    const int x3 = piece->getSquare()->getRow();
    const int y3 = piece->getSquare()->getColumn();
    const int area = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    return (area == 0);
}

/**
 * @brief Checks for a pinned piece in a list of squares.
 * @param squares The list of squares to check.
 * @param color The color of the pieces to check.
 * @return A pair of pointers to the pinned piece and the pinning piece.
 */
std::pair<Piece *, Piece *> BoardManager::checkForPinedPiece(const std::vector<Square *> &squares, const Color color) {
    Piece* pinedPiece = nullptr;
    Piece* pinningPiece = nullptr;

    for (auto& square : squares) {
        if (!square->hasPiece()) continue;

        // Check if next piece has a different color
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

        if (square->getPiece()->getType() == PieceType::QUEEN
            || square->getPiece()->getType() == PieceType::ROOK
            || square->getPiece()->getType() == PieceType::BISHOP) {

            pinningPiece = square->getPiece();
        }
        break;
    }

    if (pinedPiece != nullptr && pinningPiece != nullptr) {
        return {pinedPiece, pinningPiece};
    }

    return {nullptr, nullptr};
}

/**
 * @brief Traverses squares until a piece is found.
 * @param squares The list of squares to traverse.
 * @param color The color of the pieces to check.
 * @return A vector of pointers to the traversed squares.
 */
std::vector<Square *> BoardManager::traverseSquaresUntilPiece(const std::vector<Square *> &squares, Color color) {
    std::vector<Square*> traversedSquares;

    for (Square* square : squares) {
        if (square->hasPiece()) {
            traversedSquares.push_back(square);
            return traversedSquares;
        }
        traversedSquares.push_back(square);
    }

    return traversedSquares;
}

/**
 * @brief Gets the diagonal squares from a piece's position.
 * @param piece The piece whose position is used as the base.
 * @param up Whether to move up.
 * @param right Whether to move right.
 * @return A vector of pointers to the diagonal squares.
 */
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

/**
 * @brief Gets the straight squares from a piece's position.
 * @param piece The piece whose position is used as the base.
 * @param positivDirecion Whether to move in the positive direction.
 * @param vertical Whether to move vertically.
 * @return A vector of pointers to the straight squares.
 */
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

/**
 * @brief Gets the white pieces on the board.
 * @return A vector of pointers to the white pieces.
 */
std::vector<Piece*> BoardManager::getWhitePieces() {
    static std::vector<Piece*> whitePiecePtrs;
    whitePiecePtrs.clear();
    for (const auto& piece : whitePieces) {
        whitePiecePtrs.push_back(piece.get());
    }
    return whitePiecePtrs;
}

/**
 * @brief Gets the black pieces on the board.
 * @return A vector of pointers to the black pieces.
 */
std::vector<Piece *> BoardManager::getBlackPieces() {
    static std::vector<Piece*> blackPiecePtrs;
    blackPiecePtrs.clear();
    for (const auto& piece : blackPieces) {
        blackPiecePtrs.push_back(piece.get());
    }
    return blackPiecePtrs;
}

/**
 * @brief Removes a piece from the board.
 * @param piece The piece to remove.
 */
void BoardManager::removePiece(Piece* piece) {
    if (piece->getColor() == WHITE) {
        auto it = std::ranges::find_if(whitePieces, [piece](const std::unique_ptr<Piece>& p) { return p.get() == piece; });
        if (it != whitePieces.end()) {
            whitePieces.erase(it);
        }
    } else {
        auto it = std::ranges::find_if(blackPieces, [piece](const std::unique_ptr<Piece>& p) { return p.get() == piece; });
        if (it != blackPieces.end()) {
            blackPieces.erase(it);
        }
    }
}