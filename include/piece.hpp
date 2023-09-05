#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

class Piece
{
public:
    virtual ~Piece() = default;
	Piece(glm::vec2 p_pos, bool white);
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
	virtual void setParameters(){};
    virtual bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn);
	bool findIndMoves(std::vector<std::shared_ptr<Piece>>& Pieces, int x, int y);
	virtual void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces)= 0;
    bool isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces);
    bool findMoves(std::vector<std::shared_ptr<Piece>>& Pieces);
	std::vector<glm::vec2> legalMoves;	
	SDL_Rect getCurrentFrame();
	SDL_Rect currentFrame;
	glm::vec2 pos;
	bool white;
};
