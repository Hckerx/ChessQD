#include <glm/fwd.hpp>
#include <ostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "util.hpp"

#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "Entity.hpp"


// autosave, lines brighter, keybinds for tabs, groß und kleinschreibung bei search and replace ignorein

int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;

    if (TTF_Init() == -1)
        std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    int shortDM = DM.h * 0.9;

    RenderWindow window("Schach", shortDM);


    bool is_playing_white = true;

    int x, y;


    //TTF_Font* font128 = TTF_OpenFont("bin/debug/res/font/font.ttf", 128);
    //TTF_Font* comment = TTF_OpenFont("bin/debug/res/font/font.ttf", 32);

    // Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    /* if (window.displayWelcomeMessage(font128, comment, DM.h, DM.w, "Welcome to my Game") == 1) {
    window.cleanUp();
    // TTF_CloseFont(font32);
    // TTF_CloseFont(font24);
    //	TTF_Quit();

    TTF_Quit();
    TTF_CloseFont(comment);
    TTF_CloseFont(font128);
    SDL_Quit();
    return 0;
} */
    // 389 129
    //Board Board = Board(shortDM);


    bool gameRunning = true;

    SDL_Event event;
    // std::vector<Entity> Pieces;
    // for (int i = 0; i < 8; i++) {
    //   Pieces.push_back(Pawn({i, 1}, !is_playing_white));
    //   Pieces.push_back(Pawn({i, 6}, is_playing_white)); 
    // } 

    // Pieces.push_back(Rook({0,0}, !is_playing_white));
    // Pieces.push_back(Rook({7,0}, !is_playing_white));
    // Pieces.push_back(Rook({0,7}, is_playing_white));
    // Pieces.push_back(Rook({7,7}, is_playing_white));

    // Pieces.push_back(Knight({1, 0}, !is_playing_white));
    // Pieces.push_back(Knight({6, 0}, !is_playing_white));
    // Pieces.push_back(Knight({1, 7}, is_playing_white));
    // Pieces.push_back(Knight({6, 7}, is_playing_white));

    // 
    // Pieces.push_back(Bishop({2, 0}, !is_playing_white));
    // Pieces.push_back(Bishop({5, 0}, !is_playing_white));
    // Pieces.push_back(Bishop({2, 7}, is_playing_white));
    // Pieces.push_back(Bishop({5, 7}, is_playing_white));
    // 

    // Pieces.push_back(Queen({(is_playing_white ? 3 : 4), 0}, !is_playing_white));
    // Pieces.push_back(Queen({(is_playing_white ? 3 : 4), 7}, is_playing_white));

    // Pieces.push_back(King({(is_playing_white ? 4 : 3), 0}, !is_playing_white));
    // Pieces.push_back(King({(is_playing_white ? 4 : 3), 7}, is_playing_white));
    
    std::vector<Entity> Pieces = FenImport("r6k/pp4pp/2p5/6q1/2B1P1b1/P2P2b1/1PP3PP/5R1K");

    while (gameRunning)
    {
        window.updateWindowSize();


        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        SDL_GetMouseState(&x, &y); 
                        //selectPiece(x/window.squareSize, y/window.squareSize, &Pieces);
                        break;
                    }
                    break;
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_q: gameRunning = false; break;
                    }
            }
        }


        window.clear();
        window.renderbg();
        for (int i = 0; i < (int)Pieces.size(); i++) {
          window.render(Pieces[i]);
        }
    
          window.display();
    }
    window.cleanUp();
    // TTF_CloseFont(font32);
    // TTF_CloseFont(font24);
    //	TTF_Quit();

    //TTF_Quit();
    //TTF_CloseFont(comment);
    //TTF_CloseFont(font128);
    SDL_Quit();

    return 0;
}












/*
TTF_Font* font48 = TTF_OpenFont("res/font/font.ttf", 48);
TTF_Font* font24 = TTF_OpenFont("res/font/font.ttf", 24);
Mix_Chunk* chargeSfx = Mix_LoadWAV("res/sfx/charge.mp3");
Mix_Chunk* swingSfx = Mix_LoadWAV("res/sfx/swing.mp3");
Mix_Chunk* holeSfx = Mix_LoadWAV("res/sfx/hole.mp3");
TTF_CloseFont(font24);
SDL_Quit();

Mix_PlayChannel(-1, swingSfx, 0);
    window.renderCenter(0, 0 + 3, "POLYMARS", font32, white);
    window.renderCenter(0, 0, "POLYMARS", font32, white);

*/




//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
//
//
//




