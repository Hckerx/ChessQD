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
	int displayWelcomeMessage(TTF_Font* font128, TTF_Font* comment, int height, int width, const char* text);
	void cleanUp();
	void fullRender(std::vector<glm::vec2> highlight, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteDown);
    int squareSize;
	void updateWindowSize() {
		SDL_GetWindowSize(window, &windowx, &windowy);
        squareSize = std::min(windowx, windowy)/8;
	}
		

private:
	SDL_Texture* texture;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int windowx;
	int windowy;
	void clear();
	void render(std::shared_ptr<Piece>& p_piece, bool whiteDown);
	void renderbg(std::vector<glm::vec2> highlight, bool whiteDown);
	void display();
	SDL_Texture* loadTexture(const char* p_filePath);
};
