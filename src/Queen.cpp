#include "queen.hpp"
#include "entity.hpp"
#include "glm/glm.hpp"

#include <iostream>

Queen::Queen(glm::vec2 p_pos, bool white)
:Entity(p_pos, white){
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

void Queen::findMoves(std::vector<std::shared_ptr<Entity>>& Pieces) {
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
    int pos_x = pos[0]+1;
    for (int8_t i = pos_x; i < 8; i++)
    {
        if(!findIndMoves(Pieces,i,pos[1])){
            break;
        }
    }
    pos_x = pos[0]-1;
    for (int8_t i = pos_x; i >= 0; i--)
    {
        if(!findIndMoves(Pieces,i,pos[1])){
            break;
        }
    }
    int pos_y = pos[1]+1;
    for (int8_t i = pos_y; i < 8; i++)
    {
        if(!findIndMoves(Pieces,pos[0],i)){
            break;
        }
    }
    pos_y = pos.y - 1;
    for (int8_t i = pos_y; i >= 0; i-=1)
    {
        if(!findIndMoves(Pieces,pos[0],i)){
            break;
        }
    }
}
