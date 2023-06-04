#include "pawn.hpp"
#include "piece.hpp"

#include "util.hpp"
#include "glm/glm.hpp"
#include <iostream>
Pawn::Pawn(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){

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


void Pawn::findMoves(std::vector<std::shared_ptr<Piece>>& Pieces){
        legalMoves.clear();
        int step;
        if(white){
           step = 1;      
        }
        else{
            step = -1;
        }

        if(getMatchingPiece(glm::vec2{pos[0], pos[1]-step}, Pieces) == nullptr and !(pos[0] > 7 || pos[0] < 0 || pos[1]-step > 7|| pos[1]-step < 0)) {
                legalMoves.push_back(glm::vec2(pos[0],pos[1] - step)); 
                if (((pos.y == 6 && white) || (pos.y == 1 && white == false)) && getMatchingPiece(glm::vec2{pos[0],pos[1] - 2*step}, Pieces) == nullptr ){
                        if (!(pos[0] > 7 || pos[0] < 0 || pos[1]-2*step > 7|| pos[1]-2*step < 0)) {
                                legalMoves.push_back(glm::vec2(pos[0],pos[1] - 2*step));
                        }
                }
        } 
        
        std::shared_ptr<Piece> hypoPiece = getMatchingPiece(glm::vec2{pos[0]-1, pos[1]-step}, Pieces); 
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
