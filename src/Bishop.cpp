#include "bishop.hpp"
#include "piece.hpp"

#include <iostream>

Bishop::Bishop(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 128*2;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void Bishop::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces){
        legalMoves.clear();
        for (int8_t i = pos.x+1; i<8; i++) {
            if (!findIndMoves(Pieces, i, pos.y+(i-pos.x))) {
                break;
            }
        }
        for (int8_t i = pos.x-1; i>=0; i--) {
            if (!findIndMoves(Pieces, i, pos.y-(i-pos.x))) {
                break;
            }
        }
        for (int8_t i = pos.x+1; i<8; i++) {
            if (!findIndMoves(Pieces, i, pos.y-(i-pos.x))) {
                break;
            }
        }
        for (int8_t i = pos.x-1; i>=0; i--) {
            if (!findIndMoves(Pieces, i, pos.y+(i-pos.x))) {
                break;
            }
        }
}
