#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include <array>

#define SDL_MAIN_HANDLED

#include <SDL2/SDL_blendmode.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "renderWindow.hpp"
#include "piece.hpp"

#include "knight.hpp"
#include "queen.hpp"
#include "rook.hpp"
#include "bishop.hpp"

// Todo line width of rect
#define RESIGN 1
#define ONLINE 0

RenderWindow::RenderWindow(const char *p_title) { //SDL initiation
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;

    if (TTF_Init() == -1)
        std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;

    ChessQLDfont = TTF_OpenFont("bin/debug/res/font/REFOLTER.otf", 128);
    if (ChessQLDfont == NULL) {
        std::cerr << "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError();
    }
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int height = DM.h < DM.w ? DM.h * 0.90 : DM.w * 0.90;

    squareSize = (float) height / 8 * 0.95;
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, squareSize * 8, height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN); //| SDL_WINDOW_RESIZABLE

    if (window == NULL) {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Todo
    SDL_Surface *icon = IMG_Load("bin/debug/res/gfx/icon.png");
    SDL_SetWindowIcon(window, icon);

    SDL_ShowCursor(1);

    loadTexture("bin/debug/res/gfx/pieces.png");

}


void RenderWindow::initButtons(std::array<Button *, 3> buttons) {
    SDL_Surface *textSurface;
    SDL_Surface *textHoveredSurface;
    SDL_Texture *textTexture;
    SDL_Texture *textTextureHovered;
    for (uint8_t i = 0; i < buttons.size(); i++) {
        textSurface = TTF_RenderText_Blended(ChessQLDfont, buttons[i]->name.c_str(), buttons[i]->color);
        textHoveredSurface = TTF_RenderText_Blended(ChessQLDfont, buttons[i]->name.c_str(), buttons[i]->hoveredColor);
        if (!textSurface || !textHoveredSurface) {
            fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
        }
        textTextureHovered = SDL_CreateTextureFromSurface(renderer, textHoveredSurface);
        SDL_SetTextureBlendMode(textTextureHovered, SDL_BLENDMODE_BLEND);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
        buttons[i]->initButton(textTexture, textTextureHovered);
    }
}

void RenderWindow::freeTimer(Timer *timer) {
    //Free texture if it exists
    if (timer->texture != NULL) {
        SDL_DestroyTexture(timer->texture);
        timer->texture = NULL;
    }
}

void RenderWindow::loadFromRenderedText(Timer *timer) {

    freeTimer(timer);

    //Render text surface
    if (ChessQLDfont == nullptr) {
        std::cerr << "Font is null" << std::endl;
    }
    SDL_Surface *textSurface = TTF_RenderText_Solid(ChessQLDfont, timer->timeText.c_str(), timer->textColor);
    if (textSurface != nullptr) {
        //Create texture from surface pixels
        timer->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    } else {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
}



SDL_Texture *RenderWindow::loadTexture(const char *p_filePath) {
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == nullptr)
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

    return texture;
}

void RenderWindow::cleanUp() {

    TTF_CloseFont(ChessQLDfont);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}

void RenderWindow::render(std::shared_ptr <Piece> &p_piece, bool whiteDown) {
    SDL_Rect src;
    src.x = p_piece->getCurrentFrame().x;
    src.y = p_piece->getCurrentFrame().y;
    src.w = p_piece->getCurrentFrame().w;
    src.h = p_piece->getCurrentFrame().h;


    SDL_Rect dst;
    if (whiteDown) {
        dst.x = p_piece->getPos().x * squareSize;
        dst.y = p_piece->getPos().y * squareSize;
    } else {
        dst.x = squareSize * 8 - (p_piece->getPos().x * squareSize + squareSize);
        dst.y = squareSize * 8 - (p_piece->getPos().y * squareSize + squareSize);
    }
    dst.w = squareSize;
    dst.h = squareSize;

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

int RenderWindow::renderWidgets(std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer) {
    int i;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (i = 0; i < buttons.size(); i++) {
        buttons[i]->w = windowx / (2 * (buttons.size() + 2));
        buttons[i]->h = windowy * 0.05;
        buttons[i]->y = windowy * 0.95;
        buttons[i]->x = i * (windowx / (buttons.size() + 2)) + (windowx / (4 * (buttons.size() + 2)));

        // dst
        SDL_RenderCopy(renderer, buttons[i]->getTexture(), NULL, buttons[i]);
    }

    wTimer->w = windowx / (2 * (buttons.size() + 2));
    wTimer->h = windowy * 0.05;
    wTimer->y = windowy * 0.95;
    wTimer->x = i * (windowx / (buttons.size() + 2)) + (windowx / (4 * (buttons.size() + 2)));

    bTimer->w = windowx / (2 * (buttons.size() + 2));
    bTimer->h = windowy * 0.05;
    bTimer->y = windowy * 0.95;
    bTimer->x = (i + 1) * (windowx / (buttons.size() + 2)) + (windowx / (4 * (buttons.size() + 2)));


    SDL_RenderCopy(renderer, wTimer->texture, NULL, wTimer);
    SDL_RenderCopy(renderer, bTimer->texture, NULL, bTimer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    return 0;
}


void RenderWindow::renderbg(std::vector <glm::ivec2> highlight = {{1000, 1000}},
                            std::vector <glm::ivec2> lastMoves = {{1000, 1000}}, bool whiteDown = true) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    updateSquareSize();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            SDL_Rect rect = {j * squareSize, i * squareSize, squareSize, squareSize};
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
            for (glm::ivec2 k: lastMoves) {
                if ((whiteDown && k == glm::ivec2(j, i)) || (!whiteDown && k == glm::ivec2(8 - (j + 1), 8 - (i + 1)))) {

                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 190);
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
            for (glm::ivec2 k: highlight) {
                if ((whiteDown && k == glm::ivec2(j, i)) || (!whiteDown && k == glm::ivec2(8 - (j + 1), 8 - (i + 1)))) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}


void RenderWindow::fullRender(std::vector <glm::ivec2> highlight, std::vector <glm::ivec2> lastMoves,
                              std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteDown,
                              std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer) {
    clear();

    renderbg(highlight, lastMoves, whiteDown);
    loadFromRenderedText(wTimer);
    loadFromRenderedText(bTimer);
    renderWidgets(buttons, wTimer, bTimer);
    for (int i = 0; i < (int) Pieces.size(); i++) {
        render(Pieces[i], whiteDown);
    }

    //renderButton({"online", "resign", "turn"}) ;

}
void RenderWindow::renderTextBox(textBox& textBox) {
    TTF_SetFontSize(ChessQLDfont, 64);
    if (textBox.text.empty()) {
        textBox.text = " "; // sdl_ttf doesn't render text without any characters, so we add a space
    }
    if (ChessQLDfont == nullptr) {
        std::cerr << "failed to render text!" << std::endl;
    }
    if (renderer == nullptr) {
        std::cerr << "failed to render text!" << std::endl;
    }

    SDL_Surface *textsurface = TTF_RenderText_Blended_Wrapped(ChessQLDfont, textBox.text.c_str(), textBox.textcolor, windowx/2);
    SDL_Texture *texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
    if (textsurface == nullptr){
        std::cerr << "failed to create text surface!" << std::endl;
    }
    if (texttexture == nullptr) {
        std::cerr << "failed to create text texture!" << std::endl;
    }

    SDL_Rect textrect = {textBox.x, textBox.y, textsurface->w, textsurface->h};

    SDL_Rect rect = {textBox.x, textBox.y, windowx/2, textsurface->h};
    SDL_RenderFillRect(renderer, &rect);



    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderCopy(renderer, texttexture, nullptr, &textrect);

    SDL_FreeSurface(textsurface);
    SDL_DestroyTexture(texttexture);

    // render the cursor
    if (textBox.cursorvisible) {
        uint32_t currenttime = SDL_GetTicks();
        if (currenttime - textBox.lastcursortoggletime >= textBox.cursorblinkrate) {
            textBox.cursorvisible = !textBox.cursorvisible;
            textBox.lastcursortoggletime = currenttime;
        }

        if (textBox.cursorvisible) {
            int cursorx = textBox.x + textrect.w + 2; // adjust the cursor position as needed
            SDL_RenderDrawLine(renderer, cursorx, textBox.y + 2, cursorx, textBox.y + textrect.h - 2);
        }
    }
    SDL_RenderPresent(renderer);
    TTF_SetFontSize(ChessQLDfont, 128);
}
std::string RenderWindow::TextBox(textBox textBox) {

    while (!textBox.isDone) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // If the user closes the window, exit the loop and the function
                textBox.isDone = true;
                return "quit";
            }
            textBox.handleEvent(event);
        }
        renderTextBox(textBox);
    }
    return textBox.text;
}



// TODO: All the render functions can be summarized into one function


bool RenderWindow::displayWelcomeMessage(std::string text) {
    std::string welcomeText = text;
    SDL_Rect textRect;
    SDL_Texture *textTexture;
    SDL_Color textColor = {255, 0, 0};
    SDL_Surface *textSurface = TTF_RenderText_Blended(ChessQLDfont, welcomeText.c_str(), textColor);
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
        updateSquareSize();
        textRect.w = windowx / 1.01;
        textRect.h = windowy / 5;
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
                switch (event.key.keysym.sym) {
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
    int height = DM.h < DM.w ? DM.h * 0.9 : DM.w;
    SDL_SetWindowSize(window, height, height);
    return true;
}

int RenderWindow::displayPromotionOptions(glm::vec2 pos, bool white) {
    // we have to display a bot where are 4 options are displayed and the number returned represents the piece    
    // 1 = queen, 2 = rook, 3 = bishop, 4 = knight
    // we have to render the background first
    // create a box that is evenly spaced and display all pieces depending of is bool is set to white or not white or black



    SDL_SetRenderDrawColor(renderer, 166, 168, 171, 200);
    int calculations = (int) pos.y * squareSize - (white ? 0 : 3 * squareSize);
    SDL_Rect rect = {(int) pos.x * squareSize, calculations, (squareSize), (squareSize * 4)};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect outline = {(int) pos.x * squareSize, calculations, (squareSize), (squareSize * 4)};
    SDL_RenderDrawRect(renderer, &outline);
    int y;
    if (white) {
        y = 128;
    } else {
        y = 0;
    }
    for (int i = 128; i <= 128 * 4; i += 128) {
        SDL_Rect src;
        src.h = 128;
        src.w = 128;
        src.y = y;
        src.x = 128 * 5 - i;
        SDL_Rect dst;
        dst.x = pos.x * squareSize;
        if (white) {
            dst.y = (pos.y + (int) ((i - 128) / 128)) * squareSize;
        } else {
            dst.y = (pos.y - (int) ((i - 128) / 128)) * squareSize;
        }
        dst.w = squareSize;
        dst.h = squareSize;

        SDL_RenderCopy(renderer, texture, &src, &dst);
    }


    return 0;
}
