#include "knight.hpp"
#include "entity.hpp"
#include "SDL2/SDL.h"


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

void Knight::findMoves() {
        legalMoves.clear();
        legalMoves.push_back(glm::vec2(pos[0] + 2, pos[1] + 1));
        legalMoves.push_back(glm::vec2(pos[0] + 2, pos[1] - 1));
        legalMoves.push_back(glm::vec2(pos[0] - 2, pos[1] + 1));
        legalMoves.push_back(glm::vec2(pos[0] - 2, pos[1] - 1));
        legalMoves.push_back(glm::vec2(pos[0] + 1, pos[1] + 2));
        legalMoves.push_back(glm::vec2(pos[0] + 1, pos[1] - 2));
        legalMoves.push_back(glm::vec2(pos[0] - 1, pos[1] + 2));
        legalMoves.push_back(glm::vec2(pos[0] - 1, pos[1] - 2));
}