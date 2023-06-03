#include "entity.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <glm/detail/qualifier.hpp>
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



bool Entity::findMovesWithCheck(std::vector<std::shared_ptr<Entity>>& Pieces) {
    findMoves(Pieces);
    std::vector<glm::vec2> legalMovescopy = legalMoves;
    std::vector<glm::vec2> newLegalMoves;

    glm::vec2 oldpos = pos;
    
    glm::vec2 PfuschKoordinaten;
    for (auto move : legalMovescopy)
    {
        
        std::shared_ptr<Entity> hypoPiece = getMatchingPiece(glm::vec2{move.x, move.y}, Pieces);
        if (hypoPiece != nullptr) {
            glm::vec2 PfuschKoordinaten = hypoPiece->getPos(); // Dies sollte man niemals machen. Wir machen es trotzdem LMFAO
            hypoPiece->setPos({2000,2000});
            pos = move; //neue position setzten um damit die moves alleer pieces zu überprüfen
            if (!isKingInCheck(Pieces)) {
                newLegalMoves.push_back(move);
            }
            hypoPiece->setPos(PfuschKoordinaten);

        } else {
            pos = move;
            if (!isKingInCheck(Pieces)) {
                newLegalMoves.push_back(move);
            }
        }
    }
    pos = oldpos;
    legalMoves = newLegalMoves;
    return legalMoves.empty();
}



bool Entity::isKingInCheck(std::vector<std::shared_ptr<Entity>>& Pieces) {
    glm::vec2 kingPos;
    for (auto& i : Pieces) {
        i->findMoves(Pieces);
        std::shared_ptr<King> derivedPtr = std::dynamic_pointer_cast<King>(i); //was macht das?
        if (derivedPtr != nullptr && i->white == white) {
            kingPos = i->pos; 
        }
    }
    // for entity in pieces
    //     entity.findmoves

    for (auto& i : Pieces) {
        if (i->white != white) {
            for (glm::vec2 j : i->legalMoves) {
                if (j == kingPos) {
                    return true;
                }
            }    
        }
    }
    return false;
}
