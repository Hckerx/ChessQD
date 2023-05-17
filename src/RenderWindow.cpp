#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <string>

#include "renderWindow.hpp"
#include "entity.hpp"

RenderWindow::RenderWindow(const char* p_title)
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0) std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;

    if (TTF_Init() == -1)
        std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
	int height = DM.h < DM.w ? DM.h*0.9 : DM.w;
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, height, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}
    updateWindowSize();
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Todo
	SDL_Surface* icon = IMG_Load("bin/debug/res/gfx/icon.png");
	SDL_SetWindowIcon(window, icon);

	SDL_ShowCursor(1);
 
    squareSize = std::min(windowy, windowx)/8;
    loadTexture("bin/debug/res/gfx/pieces.png");
	}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
    SDL_Quit();
}


void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(std::shared_ptr<Entity>& p_entity, bool playing_white)
{
	SDL_Rect src; 
	src.x = p_entity->getCurrentFrame().x;
	src.y = p_entity->getCurrentFrame().y;
	src.w = p_entity->getCurrentFrame().w;
	src.h = p_entity->getCurrentFrame().h;
	

	SDL_Rect dst;
    if (playing_white) {
        dst.x = p_entity->getPos().x * squareSize;
        dst.y = p_entity->getPos().y * squareSize;
    }
    else {
        dst.x = windowx - (p_entity->getPos().x * squareSize + squareSize);
        dst.y = windowy - (p_entity->getPos().y * squareSize + squareSize);
    }
	dst.w = squareSize;
	dst.h = squareSize;

	SDL_RenderCopy(renderer, texture, &src, &dst);
}
void RenderWindow::renderbg(std::vector<glm::vec2> highlight = {{1000,1000}}, bool white_turn = true) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			SDL_Rect rect = {j * squareSize, i * squareSize, squareSize, squareSize};
			if ((i + j) % 2 == 0) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				} else {
				SDL_SetRenderDrawColor(renderer, 139,69,19, 255);
			}
			SDL_RenderFillRect(renderer, &rect);
			for (glm::vec2 k : highlight)
			{
                if (white_turn) {
                    if (k == glm::vec2(j,i) && k != glm::vec2(1000, 1000)){
                        SDL_SetRenderDrawColor(renderer, 255,255,0, 200);
                        SDL_RenderFillRect(renderer, &rect);
                    }
                    
                } else {
                    if (k == glm::vec2(8-(j+1), 8-(i+1)) && k != glm::vec2(1000, 1000)){
                        SDL_SetRenderDrawColor(renderer, 255,255,0, 200);
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
                
			}
		}
	}
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}


void RenderWindow::fullRender(std::vector<glm::vec2> highlight, std::vector<std::shared_ptr<Entity>>& Pieces, bool playing_white) {
    clear();
	renderbg(highlight, playing_white);
	for (int i = 0; i < (int)Pieces.size(); i++) {
        render(Pieces[i], playing_white);
    }
	display();
}


int RenderWindow::displayWelcomeMessage(TTF_Font* font128, TTF_Font* comment, int height, int width, const char* text) {
	SDL_Rect textRect;
	SDL_Texture* textTexture;
	SDL_Rect textRectComment;
	SDL_Texture* textTextureComment;
	SDL_Color textColor = {255, 0, 0};
	SDL_Color textColorComment = {0, 0, 0};
	SDL_Surface* textSurface = TTF_RenderText_Blended(font128, text, textColor);
	if (!textSurface) {
		fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
		return 1;
	}
	SDL_Surface* textSurfaceComment = TTF_RenderText_Blended(comment, "Press anywhere on Screen to play!", textColorComment);
	if (!textSurfaceComment) {
		fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
		return 1;
  }

  // Create a texture from the rendered text surface and set its blend mode to alpha blending
  textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

  textTextureComment = SDL_CreateTextureFromSurface(renderer, textSurfaceComment);
  SDL_SetTextureBlendMode(textTextureComment, SDL_BLENDMODE_BLEND);
  // Calculate the position of the text so it is centered on the screen
  textRect.w = textSurface->w;
  textRect.h = textSurface->h;
  textRect.x = (width - textRect.w) / 2;
  textRect.y = (height - textRect.h) / 2;

  textRectComment.w = textSurfaceComment->w;
  textRectComment.h = textSurfaceComment->h;
  textRectComment.x = (width - textRectComment.w) / 2;
  textRectComment.y = (height - textRectComment.h) / 2 + height/10;
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  SDL_RenderCopy(renderer, textTextureComment, NULL, &textRectComment);
    SDL_RenderPresent(renderer);
  // Set the flag to false initially
  bool playButtonPressed = false;

  // Loop until the "PLAY" button is pressed
  while (!playButtonPressed) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        // If the user closes the window, exit the loop and the function
        playButtonPressed = true;
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {

        // Check if the "PLAY" button was pressed
        // int x, y;
        // SDL_GetMouseState(&x, &y);
        // if (x >= textRect.x && x <= textRect.x + textRect.w && y >= textRect.y && y <= textRect.y + textRect.h) {
        //   // Set the flag to true and exit the loop
          playButtonPressed = true;
      } else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
                    {
                    case SDLK_q: 
						SDL_DestroyTexture(textTexture);
  						SDL_FreeSurface(textSurface);
						return 1;
                    }
	  }
    }

    // Render the text  
  }

  // Destroy the texture and free the surface after the loop
  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
  return 0;
}
