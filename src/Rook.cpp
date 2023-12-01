#include "rook.hpp"
#include "piece.hpp"
#include "util.hpp"

Rook::Rook(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){
        if (white)
                currentFrame.y = 128;
        else
                currentFrame.y = 0;
        currentFrame.x = 128*3;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void Rook::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
    legalMoves.clear();

    for (int8_t i = pos.x+1; i < 8; i++)
    {
        if(!findIndMoves(Pieces,i,pos.y))
            break;
    }
    for (int8_t i = pos.x-1; i >= 0; i--)
    {
        if(!findIndMoves(Pieces,i,pos.y))
            break;
    }


    for (int8_t i = pos.y+1; i < 8; i++)
    {
        if(!findIndMoves(Pieces,pos.x,i))
            break;
    }
    for (int8_t i = pos.y-1; i >= 0; i-=1)
    {
        if(!findIndMoves(Pieces,pos.x,i))
            break;
    }
}
//for castle hasMoved property must be set
bool Rook::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn, bool isPlayingOnline, bool isWhite) {
        if (Piece::move(newPos, oldPos, Pieces, whiteTurn, isPlayingOnline, isWhite)) {
                hasMoved = true;
                return true;
        } else
                return false;
}