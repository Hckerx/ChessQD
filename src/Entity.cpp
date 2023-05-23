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
    auto i = std::begin(legalMoves);
    while (i != std::end(legalMoves)) {
        glm::vec2 oldpos = pos;
        setPos(*i);
        if (isKingInCheck(Pieces)) {
            i = legalMoves.erase(i);
        }
        else {
            ++i;
        }
        setPos(oldpos);
    }
}



bool Entity::isKingInCheck(std::vector<std::shared_ptr<Entity>>& Pieces) {
    for (auto& i : Pieces) {
        i->findMoves(Pieces);
    }

    // for entity in pieces
    //     entity.findmoves
    glm::vec2 kingPos;
    for (auto& i : Pieces) {
        std::shared_ptr<King> derivedPtr = std::dynamic_pointer_cast<King>(i);
        if (derivedPtr != nullptr) {
            if (i->white == white) {
                kingPos = i->pos; 
            } 
        }
    }
    for (auto& i : Pieces) {
        for (glm::vec2 j : i->legalMoves) {
            if (j == kingPos) {
                return true;
            }
        }    
    }
    return false;
}
