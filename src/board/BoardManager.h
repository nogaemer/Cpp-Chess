//
// Created by Noah Schwenold on 1/3/2025.
//

#ifndef BOARD_MANAGER_H
#define BOARD_MANAGER_H

#include <unordered_map>
#include <memory> // For std::unique_ptr
#include <vector>

#include "../misc/Color.h"
#include "../misc/Pair.h"

class King;
class Square;
class Piece;
enum class PieceType;


class BoardManager {
public:
    BoardManager();

    static void createChessBoard();
    static Square* getSquare(const Pair& pair);
    static Piece* getPiece(PieceType type, Color color);

    static Pair getPositionByOffset(const Pair& pair, const Pair& offset);

    static std::vector<Square*> getSquaresByOffset(Piece* piece, const std::vector<Pair> &offsets);
    static bool isOnBoard(const Pair& pair);

    static King* getKing(Color color);

    static bool doesMoveRemoveCheck(Piece * piece, Square * square, King * king, Color color);

    static std::vector<Square*> validateMoves(Piece* piece, std::vector<Square*>& moves, Color color, bool checkForCheck);
    static std::pair<Piece*, Piece*> checkForPinedPiece(const std::vector<Square*>& squares, Color color);

    static std::vector<Square*> traverseSquaresUntilPiece(const std::vector<Square*>& squares, Color color);

    static std::vector<Square*> getDiagonalSquares(Piece* piece, bool up, bool right);
    static std::vector<Square*> getStraightSquares(Piece* piece, bool positivDirecion, bool vertical);

    static std::vector<Piece*> getWhitePieces();
    static std::vector<Piece*> getBlackPieces();

    static void removePiece(Piece *piece);

private:
    static King* whiteKing;
    static King* blackKing;

    static std::unordered_map<Pair, std::unique_ptr<Square>> board;
    static std::vector<std::unique_ptr<Piece>> whitePieces;
    static std::vector<std::unique_ptr<Piece>> blackPieces;

    static bool isMovePuttingKingInCheck(Piece* piece, Square* square, King* king);
    static bool areCollinear(Piece* piece, Square* square, King* king);
    static bool isAttackingPieceInMiddleOfKingAndPinnedPiece(Piece* piece, Square* square, King* king);
};

#endif //BOARD_MANAGER_H
