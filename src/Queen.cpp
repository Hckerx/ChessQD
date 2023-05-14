#include "queen.hpp"
#include "entity.hpp"
#include "SDL2/SDL.h"


#include "glm/glm.hpp"
#include <iostream>
Queen::Queen(glm::vec2 p_pos, bool white)
:Entity(p_pos, white){
        white = white;
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 4*128;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void Queen::findMoves(std::vector<Entity*> Pieces) {
        legalMoves.clear();
        for (size_t i = 0; i < 8; i++) {
                legalMoves.push_back(glm::vec2(pos[0] + i, pos[1] + i));
                legalMoves.push_back(glm::vec2(pos[0] - i, pos[1] - i));
                legalMoves.push_back(glm::vec2(pos[0] + i, pos[1] - i));
                legalMoves.push_back(glm::vec2(pos[0] - i, pos[1] + i));
                legalMoves.push_back(glm::vec2(pos[0], i));
                legalMoves.push_back(glm::vec2(i, pos[1]));
        }
}
