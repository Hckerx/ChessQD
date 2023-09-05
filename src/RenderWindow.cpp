#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_video.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <string>

#include "renderWindow.hpp"
#include "piece.hpp"

#include "knight.hpp"
#include "queen.hpp"
#include "rook.hpp"
#include "bishop.hpp"
// Todo line width of rect

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

    ChessQLDfont = TTF_OpenFont("bin/debug/res/font/REFOLTER.otf", 128);
    if (ChessQLDfont == NULL) {
        throw "Font's not working"; 
    }
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

    TTF_CloseFont(ChessQLDfont);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(std::shared_ptr<Piece>& p_piece, bool whiteDown)
{
    SDL_Rect src; 
    src.x = p_piece->getCurrentFrame().x;
    src.y = p_piece->getCurrentFrame().y;
    src.w = p_piece->getCurrentFrame().w;
    src.h = p_piece->getCurrentFrame().h;


    SDL_Rect dst;
    if (whiteDown) {
        dst.x = p_piece->getPos().x * squareSize;
        dst.y = p_piece->getPos().y * squareSize;
    }
    else {
        dst.x = squareSize*8 - (p_piece->getPos().x * squareSize + squareSize);
        dst.y = squareSize*8 - (p_piece->getPos().y * squareSize + squareSize);
    }
    dst.w = squareSize;
    dst.h = squareSize;

    SDL_RenderCopy(renderer, texture, &src, &dst);
}
void RenderWindow::renderbg(std::vector<glm::ivec2> highlight = {{1000,1000}}, std::vector<glm::ivec2> lastMoves = {{1000, 1000}}, bool whiteDown=true) {
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
            for (glm::ivec2 k : lastMoves)
            {
                if (whiteDown) {
                    if (k == glm::ivec2(j,i) && k != glm::ivec2(1000, 1000)){
                        SDL_SetRenderDrawColor(renderer, 255,255,0, 120);
                        SDL_RenderFillRect(renderer, &rect);
                    }

                } else {
                    if (k == glm::ivec2(8-(j+1), 8-(i+1)) && k != glm::ivec2(1000, 1000)){
                        SDL_SetRenderDrawColor(renderer, 255,255,0, 120);
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }

            }
            for (glm::ivec2 k : highlight)
            {
                if (whiteDown) {
                    if (k == glm::ivec2(j,i) && k != glm::ivec2(1000, 1000)){

                        SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
                        SDL_RenderFillRect(renderer, &rect);
                    }

                } else {
                    if (k == glm::ivec2(8-(j+1), 8-(i+1)) && k != glm::ivec2(1000, 1000)){
                        SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
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


void RenderWindow::fullRender(std::vector<glm::ivec2> highlight, std::vector<glm::ivec2> lastMoves, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteDown) {
    clear();

    renderbg(highlight, lastMoves,  whiteDown);
    for (int i = 0; i < (int)Pieces.size(); i++) {
        render(Pieces[i], whiteDown);
    }
}


bool RenderWindow::displayWelcomeMessage(std::string text) {
    std::string welcomeText = text;
    SDL_Rect textRect;
    SDL_Texture* textTexture;
    SDL_Color textColor = {255, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Blended(ChessQLDfont, welcomeText.c_str(), textColor);
    if (!textSurface) {
        fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
        return 1;
    }

    // Create a texture from the rendered text surface and set its blend mode to alpha blending
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

    // Calculate the position of the text so it is centered on the screen

    // Set the flag to false initially
    bool playButtonPressed = false;

    // Loop until the "PLAY" button is pressed
    while (!playButtonPressed) {
        updateWindowSize();
        textRect.w = windowx/1.01;
        textRect.h = windowy/5;
        textRect.x = (windowx - textRect.w) / 2;
        textRect.y = (windowy - textRect.h) / 2;
        // dst
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);
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
                    return false;
                }
            }
        }

        // Render the text  
    }

    // Destroy the texture and free the surface after the loop
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int height = DM.h < DM.w ? DM.h*0.9 : DM.w;
    SDL_SetWindowSize(window, height, height);  
    return true;
}

int RenderWindow::displayPromotionOptions(glm::vec2 pos, bool white) {
    // we have to display a bot where are 4 options are displayed and the number returned represents the piece    
    // 1 = queen, 2 = rook, 3 = bishop, 4 = knight
    // we have to render the background first
    // create a box that is evenly spaced and display all pieces depending of is bool is set to white or not white or black



    SDL_SetRenderDrawColor(renderer, 166, 168, 171, 255);
    int calculations = (int)pos.y*squareSize - (white ? 0 : 3*squareSize);
    SDL_Rect rect = {(int)pos.x*squareSize, calculations, (squareSize ), (squareSize * 4)};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect outline = {(int)pos.x*squareSize, calculations, (squareSize ), (squareSize * 4)};
    SDL_RenderDrawRect(renderer, &outline);
    int y;
    if (white) {
        y = 128;
    }
    else {
        y = 0;
    }
    for (int i = 128; i<=128*4; i+=128) {
        SDL_Rect src; 
        src.h = 128;
        src.w = 128;
        src.y = y; 
        src.x = 128*5-i; 
        SDL_Rect dst;
        dst.x = pos.x* squareSize;
        if (white) {
            dst.y = (pos.y+(int)((i-128)/128)) * squareSize;
        } else {
            dst.y = (pos.y-(int)((i-128)/128)) * squareSize;
        }
        dst.w = squareSize;
        dst.h = squareSize;

        SDL_RenderCopy(renderer, texture, &src, &dst);
    }


    return 0;
}
