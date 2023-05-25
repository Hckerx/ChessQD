#include "entity.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <ctime>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include <algorithm>
#include <king.hpp>

Entity::Entity(glm::vec2 p_pos, bool white)
:pos(p_pos), white(white)
{
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}

bool Entity::findIndMoves(std::vector<std::shared_ptr<Entity>>& Pieces, int x, int y){
    std::shared_ptr<Entity> hypotheticalPiece = getMatchingPiece(glm::vec2{x, y}, Pieces);
    if (x > 7 || x < 0 || y > 7 || y < 0) {
        return false;
    }
    if (hypotheticalPiece == nullptr)
    {
        legalMoves.push_back(glm::vec2(x, y));
        return true;
    }
    else if (hypotheticalPiece->white == white)
    {
        return false;
    }
    else
    {
        legalMoves.push_back(glm::vec2(x, y));
        return false;
    }
}

bool Entity::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Entity>>& Pieces, bool white_turn) {
    if (white_turn == white) {
        for (glm::vec2 i: legalMoves) {
            if (i == newPos)	{
                std::shared_ptr<Entity> hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y}, Pieces);
                if (hypoPiece != nullptr) {
                    std::vector<std::shared_ptr<Entity>>::iterator position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()){ // == myVector.end() means the element was not found
                        Pieces.erase(position);
                    }
                }
                setPos(newPos);
                // for (auto& i : Pieces) {
                //     i->legalMoves.clear();
                // }
                // for (auto& i : Pieces) {
                //     i->findMoves(Pieces);
                // }
                // for (auto& i : Pieces) {
                //     i->findMoves(Pieces);
                // }
                return true;

            }
        }
    }
    setPos(oldPos);
    return false;
}



void Entity::findMovesWithCheck(std::vector<std::shared_ptr<Entity>>& Pieces) {
    findMoves(Pieces);
    std::cout << "----------------- moves for the entity ---------------" << std::endl; 
    for (auto i : legalMoves)
    {
        std::cout << glm::to_string(i) << std::endl; 
    }
    std::cout << "----------------- moves for the entity ---------------" << std::endl; 
    for (auto i = legalMoves.begin(); i!=legalMoves.end(); i++) {
        glm::vec2 oldpos = pos;
        //pos.x = (int)i->x; //neue position setzten um damit die moves alleer pieces zu überprüfen
        //pos.y = (int)i->y;
        
        std::cout << "print *i" << std::endl;
        std::cout << i->x << ", " << i->y << std::endl;
        std::cout << "print *i finished" << std::endl;

        if (isKingInCheck(Pieces)) {
            std::cout << "entered if" << std::endl;
            i = legalMoves.erase(i);
            if (legalMoves.empty()) {
                pos = oldpos;
                return;
            }
        }
        pos = oldpos;
        std::cout << "print oldpos" << std::endl;
        std::cout << i->x << ", " << i->y << std::endl;
        std::cout << "print oldpos finished" << std::endl;
    }
}



bool Entity::isKingInCheck(std::vector<std::shared_ptr<Entity>>& Pieces) {
    std::cout << "check function has been activated" << std::endl;
    glm::vec2 kingPos;
    for (auto& i : Pieces) {
        i->findMoves(Pieces);
        std::shared_ptr<King> derivedPtr = std::dynamic_pointer_cast<King>(i); //was macht das?
        if (derivedPtr != nullptr and i->white == white) {
                kingPos = i->pos; 
        }
    }
    // for entity in pieces
    //     entity.findmoves

    for (auto& i : Pieces) {
        for (glm::vec2 j : i->legalMoves) {
            if (j == kingPos) {
                std::cout << glm::to_string(i->pos) << "with move --> " << glm::to_string(j) <<  std::endl; 
                return true;
            }
        }    
    }
    return false;
}
