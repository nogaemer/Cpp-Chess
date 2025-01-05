//
// Created by Noah Schwenold on 1/5/2025.
//

#ifndef MOVE_UPDATER_H
#define MOVE_UPDATER_H
#include <vector>

#include "../board/Display.h"

class Piece;
class Square;

class MoveUpdater {
public:
    void registerPiece(Piece* piece);
    void unregisterPiece(Piece* piece);
    std::vector<Piece*>* getRegisteredPieces();

    static void removePieceFromUpdates(Piece * piece);

    static void update(Piece *piece, std::vector<Square *> *updates);
    static void updateAll(Square* oldSquare, Piece* piece);
private:
    std::vector<Piece*> pieces{};
};

#endif //MOVE_UPDATER_H
