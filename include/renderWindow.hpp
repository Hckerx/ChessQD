#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "piece.hpp"


class RenderWindow 
{
public:
	RenderWindow(const char* p_title);
	bool displayWelcomeMessage(std::string text);
	void cleanUp();
	void fullRender(std::vector<glm::ivec2> highlight, std::vector<glm::ivec2> lastMoves, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteDown, bool over[2]);
    int displayPromotionOptions(glm::vec2 pos, bool white);
    int squareSize;
	void updateSquareSize() {
		SDL_GetWindowSize(window, &windowx, &windowy);
        squareSize = std::min(windowx, windowy)/8;
	}
	void display();
	bool checkIfButtonClicked(std::uint8_t buttonType, glm::ivec2 mousepos);

private:
	int createButton(std::string buttons[3], bool over);

    SDL_Rect textRectResign;
    SDL_Rect textRectOnline;
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
