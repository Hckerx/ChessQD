#include "pawn.hpp"
#include "entity.hpp"
#include "SDL2/SDL.h"


#include "glm/glm.hpp"
#include <iostream>
Pawn::Pawn(glm::vec2 p_pos, bool white)
:Entity(p_pos, white){
        white = white;
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 0;
        currentFrame.w = 128;
        currentFrame.h = 128;

}
void Pawn::findMoves(){
        legalMoves.clear();
        //Fixme handle takes and blockade, and pawn +2 at start and promoting????
        if (hasMoved) {

        }
        if (white) {
                legalMoves.push_back(glm::vec2(pos[0],pos[1] - 1));
        }
        else {
                legalMoves.push_back(glm::vec2(pos[0],pos[1] + 1));
        }
}
       
  
bool Pawn::move(glm::vec2 newPos, glm::vec2 oldPos) {
        if (Entity::move(newPos, oldPos)) {
                hasMoved = true;
                return true;
        }
        return false;
}







