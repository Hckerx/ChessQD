#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <glm/glm.hpp>

Entity::Entity(glm::vec2 p_pos)
:pos(p_pos)
{
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}

