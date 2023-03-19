#include <glm/fwd.hpp>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "pawn.hpp"
#include "Entity.hpp"

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


    SDL_Texture* Textures = window.loadTexture("bin/debug/res/gfx/pieces.png");
    bool gameRunning = true;

    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    while (gameRunning)
    {
        window.updateWindowSize();
        int squareSize = std::min(window.windowx, window.windowy)/8;
        Entity black_Pawn_1 = Pawn({squareSize * 0, squareSize}, Textures, false);
        Entity black_Pawn_2 = Pawn({squareSize * 1, squareSize}, Textures, false);
        Entity black_Pawn_3 = Pawn({squareSize * 2, squareSize}, Textures, false);
        Entity black_Pawn_4 = Pawn({squareSize * 3, squareSize}, Textures, false);
        Entity black_Pawn_5 = Pawn({squareSize * 4, squareSize}, Textures, false);
        Entity black_Pawn_6 = Pawn({squareSize * 5, squareSize}, Textures, false);
        Entity black_Pawn_7 = Pawn({squareSize * 6, squareSize}, Textures, false);
        Entity black_Pawn_8 = Pawn({squareSize * 7, squareSize}, Textures, false);


        Entity white_Pawn_1 = Pawn({squareSize * 0, squareSize * 6}, Textures, true);
        Entity white_Pawn_2 = Pawn({squareSize * 1, squareSize * 6}, Textures, true);
        Entity white_Pawn_3 = Pawn({squareSize * 2, squareSize * 6}, Textures, true);
        Entity white_Pawn_4 = Pawn({squareSize * 3, squareSize * 6}, Textures, true);
        Entity white_Pawn_5 = Pawn({squareSize * 4, squareSize * 6}, Textures, true);
        Entity white_Pawn_6 = Pawn({squareSize * 5, squareSize * 6}, Textures, true);
        Entity white_Pawn_7 = Pawn({squareSize * 6, squareSize * 6}, Textures, true);
        Entity white_Pawn_8 = Pawn({squareSize * 7, squareSize * 6}, Textures, true);


        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
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
        window.render(white_Pawn_1, {0,0});
        window.render(white_Pawn_2, {0,0});
        window.render(white_Pawn_3, {0,0});
        window.render(white_Pawn_4, {0,0});
        window.render(white_Pawn_5, {0,0});
        window.render(white_Pawn_6, {0,0});
        window.render(white_Pawn_7, {0,0});
        window.render(white_Pawn_8, {0,0});
        window.render(black_Pawn_1, {0,0});
        window.render(black_Pawn_2, {0,0});
        window.render(black_Pawn_3, {0,0});
        window.render(black_Pawn_4, {0,0});
        window.render(black_Pawn_5, {0,0});
        window.render(black_Pawn_6, {0,0});
        window.render(black_Pawn_7, {0,0});
        window.render(black_Pawn_8, {0,0});
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
    window.renderCenter(0, 0 + 3, "POLYMARS", font32, black);
    window.renderCenter(0, 0, "POLYMARS", font32, white);

*/




//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
