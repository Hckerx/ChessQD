#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <glm/glm.hpp>

class Entity
{
public:
	Entity(glm::vec2 p_pos);
	glm::vec2& getPos()
	{
		return pos;
	}
	glm::vec2 getPosCopy()
	{
		return pos;
	}
	void setPos(glm::vec2 posold)
	{
		pos.x = posold.x;
		pos.y = posold.y;
	}

	virtual void highlightPiece() =0;
	SDL_Rect getCurrentFrame();
	SDL_Rect currentFrame;
	glm::vec2 pos;
};
