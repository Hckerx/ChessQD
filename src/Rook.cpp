#include "rook.hpp"
#include "entity.hpp"
#include "SDL2/SDL.h"


#include "glm/glm.hpp"
#include <iostream>
Rook::Rook(glm::vec2 p_pos, bool white)
:Entity(p_pos){
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

void Rook::findMoves() {
        for (size_t i = 0; i < 8; i++)
        {
                legalMoves.push_back(glm::vec2(pos[0], i));
                legalMoves.push_back(glm::vec2(i, pos[1]));
                
        }
}