#include "rook.hpp"
#include "piece.hpp"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "util.hpp"
#include <iostream>
Rook::Rook(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 128*3;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void Rook::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
    legalMoves.clear();
    // erst x
    int pos_x = pos.x+1;
    for (int8_t i = pos_x; i < 8; i++)
    {
        if(!findIndMoves(Pieces,i,pos.y)){
            break;
        }
    }
    pos_x = pos.x-1;
    for (int8_t i = pos_x; i >= 0; i--)
    {
        if(!findIndMoves(Pieces,i,pos.y)){
            break;
        }
    }
    int pos_y = pos.y+1;
    for (int8_t i = pos_y; i < 8; i++)
    {
        if(!findIndMoves(Pieces,pos.x,i)){
            break;
        }
    }
    pos_y = pos.y - 1;
    for (int8_t i = pos_y; i >= 0; i-=1)
    {
        if(!findIndMoves(Pieces,pos.x,i)){
            break;
        }
    }
}


bool Rook::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn, bool isPlayingOnline, bool isWhite) {
        if (Piece::move(newPos, oldPos, Pieces, whiteTurn, isPlayingOnline, isWhite)) {
                hasMoved = true;
                return true;
        } else {
                return false;
        }
}
