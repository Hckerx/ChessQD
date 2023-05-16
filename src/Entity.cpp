#include "entity.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <glm/glm.hpp>
#include <algorithm>

Entity::Entity(glm::vec2 p_pos, bool white)
:pos(p_pos), white(white)
{
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}

bool Entity::findIndMoves(std::vector<Entity *> Pieces, int x, int y){
    Entity *hypotheticalPiece = getMatchingPiece(x, y, Pieces);
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

bool Entity::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<Entity*> Pieces, bool white_turn) {
    if (white_turn == white) {
        for (glm::vec2 i: legalMoves) {
            if (i == newPos)	{
                Entity* hypoPiece = getMatchingPiece(newPos.x, newPos.y, Pieces);
                if (hypoPiece != nullptr) {
                    std::vector<Entity*>::iterator position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()){ // == myVector.end() means the element was not found
                        delete *position;
                        Pieces.erase(position);
                    }
                }
                setPos(newPos);
                return true;
            }
        }
    }
    setPos(oldPos);
    return false;
}