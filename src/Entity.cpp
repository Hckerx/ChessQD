#include "entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <glm/glm.hpp>
#include "util.hpp"

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


