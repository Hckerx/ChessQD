#include "king.hpp"
#include "piece.hpp"
#include "glm/glm.hpp"

#include <iostream>
#include <memory>
King::King(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 128*5;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void King::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
        //FIXME add special Rules rochade schach schachmatt?????
        legalMoves.clear();
        findIndMoves(Pieces, pos[0] + 1, pos[1] + 1);
        findIndMoves(Pieces, pos[0] + 1, pos[1] - 1);
        findIndMoves(Pieces, pos[0] - 1, pos[1] + 1);
        findIndMoves(Pieces, pos[0] - 1, pos[1] - 1);
        findIndMoves(Pieces, pos[0], pos[1] - 1);
        findIndMoves(Pieces, pos[0], pos[1] + 1);
        findIndMoves(Pieces, pos[0] + 1, pos[1]);
        findIndMoves(Pieces, pos[0] - 1, pos[1]);

}


