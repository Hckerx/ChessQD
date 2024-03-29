#include "queen.hpp"

/**
* constructor for the piece
* define the queens position on sprite in bin/debug/res/gfx/pieces.png
* @param p_pos the position of the piece
* @param white the color of the piece
*/
Queen::Queen(glm::vec2 p_pos, bool white)
        : Piece(p_pos, white) {
    if (white) {
        currentFrame.y = 128;
    } else {
        currentFrame.y = 0;
    }
    currentFrame.x = 4 * 128;
    currentFrame.w = 128;
    currentFrame.h = 128;
}
/* finds all possible moves of the queen
* @param Pieces a vector containing all pieces
*/
void Queen::findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) {
    legalMoves.clear();
    for (int8_t i = pos.x + 1; i < 8; i++) {
        if (!findIndMoves(Pieces, i, pos.y + (i - pos.x))) {
            break;
        }
    }
    for (int8_t i = pos.x - 1; i >= 0; i--) {
        if (!findIndMoves(Pieces, i, pos.y - (i - pos.x))) {
            break;
        }
    }
    for (int8_t i = pos.x + 1; i < 8; i++) {
        if (!findIndMoves(Pieces, i, pos.y - (i - pos.x))) {
            break;
        }
    }
    for (int8_t i = pos.x - 1; i >= 0; i--) {
        if (!findIndMoves(Pieces, i, pos.y + (i - pos.x))) {
            break;
        }
    }
    int pos_x = pos.x + 1;
    for (int8_t i = pos_x; i < 8; i++) {
        if (!findIndMoves(Pieces, i, pos.y)) {
            break;
        }
    }
    pos_x = pos.x - 1;
    for (int8_t i = pos_x; i >= 0; i--) {
        if (!findIndMoves(Pieces, i, pos.y)) {
            break;
        }
    }
    int pos_y = pos.y + 1;
    for (int8_t i = pos_y; i < 8; i++) {
        if (!findIndMoves(Pieces, pos.x, i)) {
            break;
        }
    }
    pos_y = pos.y - 1;
    for (int8_t i = pos_y; i >= 0; i -= 1) {
        if (!findIndMoves(Pieces, pos.x, i)) {
            break;
        }
    }
}
