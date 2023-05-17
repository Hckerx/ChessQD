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
                return true;
            }
        }
    }
    setPos(oldPos);
    return false;
}
