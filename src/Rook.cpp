#include "rook.hpp"
#include "piece.hpp"
#include "util.hpp"

/**
* constructor for the Rook class which inherits from the Piece class
* sets the position(in the sprite) and color of the Rook
* @param p_pos
* @param white
*/
Rook::Rook(glm::vec2 p_pos, bool white)
        : Piece(p_pos, white) {
    if (white)
        currentFrame.y = 128;
    else
        currentFrame.y = 0;
    currentFrame.x = 128 * 3;
    currentFrame.w = 128;
    currentFrame.h = 128;
}
/**
* Function that finds all the legal moves of the Rook
* @param Pieces
* @return void
*/
void Rook::findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) {
    legalMoves.clear();

    for (int8_t i = pos.x + 1; i < 8; i++) {
        if (!findIndMoves(Pieces, i, pos.y))
            break;
    }
    for (int8_t i = pos.x - 1; i >= 0; i--) {
        if (!findIndMoves(Pieces, i, pos.y))
            break;
    }

    for (int8_t i = pos.y + 1; i < 8; i++) {
        if (!findIndMoves(Pieces, pos.x, i))
            break;
    }
    for (int8_t i = pos.y - 1; i >= 0; i -= 1) {
        if (!findIndMoves(Pieces, pos.x, i))
            break;
    }
}

/* Function to move the Rook
* Must overload function to set has Moved to true 
* @param newPos
* @param oldPos
* @param Pieces
* @param whiteTurn
* @param isPlayingOnline
* @param isWhite
* @return bool if the move was successful
*/
bool Rook::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
                bool isPlayingOnline, bool isWhite) {
    if (Piece::move(newPos, oldPos, Pieces, whiteTurn, isPlayingOnline, isWhite)) {
        hasMoved = true;
        return true;
    } else
        return false;
}
