#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "piece.hpp"


class RenderWindow 
{
public:
	RenderWindow(const char* p_title);
	bool displayWelcomeMessage(std::string text);
	void cleanUp();
	void fullRender(std::vector<glm::ivec2> highlight, std::vector<glm::ivec2> lastMoves, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteDown);
    int displayPromotionOptions(glm::vec2 pos, bool white);
    int squareSize;
	void updateWindowSize() {
		SDL_GetWindowSize(window, &windowx, &windowy);
        squareSize = std::min(windowx, windowy)/8;
	}
	void display();
		

private:
	SDL_Texture* texture;
	SDL_Window* window;
	SDL_Renderer* renderer;
    TTF_Font* ChessQLDfont;
	int windowx;
	int windowy;
	void clear();
	void render(std::shared_ptr<Piece>& p_piece, bool whiteDown);
	void renderbg(std::vector<glm::ivec2> highlight, std::vector<glm::ivec2> lastMoves, bool whiteDown);
	SDL_Texture* loadTexture(const char* p_filePath);
};
