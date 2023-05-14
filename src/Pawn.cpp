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
        lastPos = pos;

}
void Pawn::findMoves(std::vector<Entity*> Pieces){
        
        
        legalMoves.clear();
        //Fixme handle takes and blockade, and pawn +2 at start and promoting????
        if (lastPos != pos)
        {
            lastPos = {1000,1000};
            if (white) {
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] - 1));
                }
                else {
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] + 1));
                }
        }
        else{
                if (white) {
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] - 1));
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] - 2));
                }
                else {
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] + 1));
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] + 2));
                }
        }                      
}
       
  

