#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

class Entity
{
public:
    virtual ~Entity() = default;
	Entity(glm::vec2 p_pos, bool white);
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
    bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Entity>>& Pieces, bool white_turn);
	bool findIndMoves(std::vector<std::shared_ptr<Entity>>& Pieces, int x, int y);
	virtual void findMoves(std::vector<std::shared_ptr<Entity>>& Pieces)= 0;
    bool isKingInCheck(std::vector<std::shared_ptr<Entity>>& Pieces);
    void findMovesWithCheck(std::vector<std::shared_ptr<Entity>>& Pieces);
	std::vector<glm::vec2> legalMoves;	
	SDL_Rect getCurrentFrame();
	SDL_Rect currentFrame;
	glm::vec2 pos;
	bool white;
};
