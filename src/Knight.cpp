#include "knight.hpp"
#include "entity.hpp"


#include "glm/glm.hpp"
#include <iostream>
Knight::Knight(glm::vec2 p_pos, bool white)
:Entity(p_pos, white){
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 128;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void Knight::findMoves(std::vector<std::shared_ptr<Entity>>& Pieces) {
        legalMoves.clear();
        findIndMoves(Pieces, pos[0] + 2, pos[1] + 1);
        findIndMoves(Pieces, pos[0] + 2, pos[1] - 1);
        findIndMoves(Pieces, pos[0] - 2, pos[1] + 1);
        findIndMoves(Pieces, pos[0] - 2, pos[1] - 1);
        findIndMoves(Pieces, pos[0] + 1, pos[1] + 2);
        findIndMoves(Pieces, pos[0] + 1, pos[1] - 2);
        findIndMoves(Pieces, pos[0] - 1, pos[1] + 2);
        findIndMoves(Pieces, pos[0] - 1, pos[1] - 2);
}


