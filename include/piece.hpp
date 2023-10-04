#pragma once //???
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Piece
{
public:
    virtual ~Piece() = default;
	Piece(std::array<int, 2> p_pos, bool white);
	std::array<int, 2>& getPos()
	{
		return pos;
	}
	std::array<int, 2> getPosCopy()
	{
		return pos;
	}
	void setPos(std::array<float, 2> posold)
	{
		pos[0] = posold[0];
		pos[1] = posold[1];
	}
	virtual void setParameters(){};
    virtual bool move(std::array<int, 2> newPos, std::array<int, 2> oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn,bool isPlayingOnline, bool isWhite);
	bool findIndMoves(std::vector<std::shared_ptr<Piece>>& Pieces, int x, int y);
	virtual void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces)= 0;
    bool isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces);
    bool findMoves(std::vector<std::shared_ptr<Piece>>& Pieces);
	std::vector<std::array<int, 2>> legalMoves;	
	SDL_Rect getCurrentFrame();
	SDL_Rect currentFrame;
	std::array<int, 2> pos;
	bool white;
};
