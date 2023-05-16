#include "pawn.hpp"
#include "entity.hpp"

#include "util.hpp"
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
        int step;
        if(white){
           step = 1;      
        }
        else{
            step = -1;
        }

        // Check if pawn has moved. If  it has set it to 1000 so it cannot move twice any more. 
        if (lastPos != pos && lastPos != glm::vec2{1000,1000})
            lastPos = {1000,1000};
        if(getMatchingPiece(pos[0], pos[1]-step, Pieces) == nullptr) {
                legalMoves.push_back(glm::vec2(pos[0],pos[1] - step)); 
                
                if (lastPos == pos && getMatchingPiece(pos[0],pos[1] - 2*step, Pieces) == nullptr ){
                        legalMoves.push_back(glm::vec2(pos[0],pos[1] - 2*step));
                }
        } 
        
        Entity* hypoPiece = getMatchingPiece(pos[0]-1, pos[1]-step, Pieces); 
        if (hypoPiece != nullptr) {
                if (hypoPiece->white != white) {
                        legalMoves.push_back(glm::vec2(pos[0]-1, pos[1]-step));
                }
        }
        hypoPiece = getMatchingPiece(pos[0]+1, pos[1]-step, Pieces); 
        if (hypoPiece != nullptr) {
                if (hypoPiece->white != white) {
                        legalMoves.push_back(glm::vec2(pos[0]+1, pos[1]-step));
                }
        }                         
}