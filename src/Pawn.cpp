#include "pawn.hpp"
#include "entity.hpp"

#include "util.hpp"
#include "glm/glm.hpp"
#include <iostream>
Pawn::Pawn(glm::vec2 p_pos, bool white)
:Entity(p_pos, white){

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


void Pawn::findMoves(std::vector<std::shared_ptr<Entity>>& Pieces){
        legalMoves.clear();
        int step;
        if(white){
           step = 1;      
        }
        else{
            step = -1;
        }

        // Check if pawn has moved. If  it has set it to 1000 so it cannot move twice any more. 
        if (lastPos != pos && lastPos != glm::vec2{1000,1000}) {
            lastPos = {1000,1000};
            isEnPassantVulnerable = true;
        }
        if(getMatchingPiece(glm::vec2{pos[0], pos[1]-step}, Pieces) == nullptr and !(pos[0] > 7 || pos[0] < 0 || pos[1]-step > 7|| pos[1]-step < 0)) {
                legalMoves.push_back(glm::vec2(pos[0],pos[1] - step)); 
                
                if (lastPos == pos && getMatchingPiece(glm::vec2{pos[0],pos[1] - 2*step}, Pieces) == nullptr ){
                        if (!(pos[0] > 7 || pos[0] < 0 || pos[1]-2*step > 7|| pos[1]-2*step < 0)) {
                                legalMoves.push_back(glm::vec2(pos[0],pos[1] - 2*step));
                        }
                }
        } 
        
        std::shared_ptr<Entity> hypoPiece = getMatchingPiece(glm::vec2{pos[0]-1, pos[1]-step}, Pieces); 
        if (hypoPiece != nullptr) {
                if (hypoPiece->white != white && !(pos[0]-1 > 7 || pos[0]-1 < 0 || pos[1]-step > 7 || pos[1]-step < 0)) {
                        legalMoves.push_back(glm::vec2(pos[0]-1, pos[1]-step));
                }
        }
        hypoPiece = getMatchingPiece(glm::vec2{pos[0]+1, pos[1]-step}, Pieces); 
        if (hypoPiece != nullptr) {
                if (hypoPiece->white != white && !(pos[0]+1 > 7 || pos[0]+1 < 0 || pos[1]+step > 7 || pos[1]+step < 0)) {
                        legalMoves.push_back(glm::vec2(pos[0]+1, pos[1]-step));
                }
        }                         
        // Implement En passant
        
    //
}
