#include "knight.hpp"
#include "piece.hpp"

Knight::Knight(glm::vec2 p_pos, bool white)
        : Piece(p_pos, white) {
    if (white)
        currentFrame.y = 128;
    else
        currentFrame.y = 0;
    currentFrame.x = 128;
    currentFrame.w = 128;
    currentFrame.h = 128;
}

void Knight::findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) {
    legalMoves.clear();
    findIndMoves(Pieces, pos.x + 2, pos.y + 1);
    findIndMoves(Pieces, pos.x + 2, pos.y - 1);
    findIndMoves(Pieces, pos.x - 2, pos.y + 1);
    findIndMoves(Pieces, pos.x - 2, pos.y - 1);
    findIndMoves(Pieces, pos.x + 1, pos.y + 2);
    findIndMoves(Pieces, pos.x + 1, pos.y - 2);
    findIndMoves(Pieces, pos.x - 1, pos.y + 2);
    findIndMoves(Pieces, pos.x - 1, pos.y - 2);
}