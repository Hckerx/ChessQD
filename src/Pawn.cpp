#include "pawn.hpp"
#include "piece.hpp"

#include "util.hpp"
#include <algorithm>

Pawn::Pawn(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){

        if (white) {
                currentFrame.y = 128;
                 step = 1;
        }
        else {
                currentFrame.y = 0;
                step = -1;
        }
        currentFrame.x = 0;
        currentFrame.w = 128;
        currentFrame.h = 128;

}


void Pawn::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces){
        legalMoves.clear();
       
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
        hypoPiece = getMatchingPiece(glm::vec2{pos[0] - 1, pos[1]}, Pieces);
        std::shared_ptr<Pawn> hypoPawn = std::dynamic_pointer_cast<Pawn>(hypoPiece); // was macht das?
        if (hypoPawn != nullptr) {
                if (hypoPawn->isEnPassantVulnerable) {
                        legalMoves.push_back(glm::vec2{pos[0] - 1, pos[1]-step});
                }
        }

        hypoPiece = getMatchingPiece(glm::vec2{pos[0] + 1, pos[1]}, Pieces);
        hypoPawn = std::dynamic_pointer_cast<Pawn>(hypoPiece); // was macht das?
        if (hypoPawn != nullptr) {
                if (hypoPawn->isEnPassantVulnerable) {
                        legalMoves.push_back(glm::vec2{pos[0] + 1, pos[1]-step});
                }
        }
}


bool Pawn::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn) {
    std::shared_ptr<Piece> hypoPiece;

    if (whiteTurn == white) {
        for (glm::vec2 i: legalMoves) {
            if (i == newPos)	{
                if ((white && oldPos.y == 6 && newPos.y == 4) || (!white && oldPos.y == 1 && newPos.y == 3)) {
                    isEnPassantVulnerable = true; }

                if (oldPos.x != newPos.x) {
                    hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y}, Pieces); 
                    if (hypoPiece == nullptr) {
                        hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y+step}, Pieces);
                    }

                    std::vector<std::shared_ptr<Piece>>::iterator position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()){ // == myVector.end() means the element was not found
                        Pieces.erase(position);
                    }

                    }
                    for (auto &i : Pieces)
                    {
                        if (this == i.get()) {
                            continue;
                        }
                        std::shared_ptr<Pawn> hypoPawn = std::dynamic_pointer_cast<Pawn>(i);
                        if (hypoPawn != nullptr) {
                            
                            hypoPawn->isEnPassantVulnerable = false;
                        }
                    }
                    setPos(newPos);
                    return true;
                }
            }  
        setPos(oldPos);
        return false;     
    }
    setPos(oldPos);
    return false;
}
