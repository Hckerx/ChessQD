#define SDL_MAIN_HANDLED
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <glm/fwd.hpp>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "renderWindow.hpp"
#include "knight.hpp"
#include "queen.hpp"
#include "rook.hpp"
#include "bishop.hpp"

// define some constants
#define RESIGN 1
#define ONLINE 0

/** constructor for the renderwindow
* @param p_title the title of the window
*/
RenderWindow::RenderWindow(const char *p_title) { //SDL initiation
    // boilerplate code for SDL initiation
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;

    if (TTF_Init() == -1)
        std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;

    // font
    ChessQLDfont = TTF_OpenFont("bin/debug/res/font/REFOLTER.otf", 128);
    if (ChessQLDfont == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError();
    }
    // works on all monitors vertical or horizontal since bigger side is used
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int height = DM.h < DM.w ? DM.h * 0.90 : DM.w * 0.90;
    // calculate square size
    squareSize = (float) height / 8 * 0.95;
    // create window
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, squareSize * 8, height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
    }
    // renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // blendmode to allow transparency, e.g. promotion view
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // TODO: set icon
    //SDL_Surface *icon = IMG_Load("bin/debug/res/gfx/icon.png"); FIXME
    //SDL_SetWindowIcon(window, icon); FIXME
    // show cursor
    SDL_ShowCursor(1);
    // load sprite
    loadTexture("bin/debug/res/gfx/pieces.png");

}

/**
* Function to initialize the buttons
* @param buttons an array of buttons
* @return void
*/
void RenderWindow::initButtons(std::array<Button *, 3> buttons) {
    SDL_Surface *textSurface;
    SDL_Surface *textHoveredSurface;
    SDL_Texture *textTexture;
    SDL_Texture *textTextureHovered;
    // loop through create the texture from surface and set the appropiate members in the button object
    for (auto & button : buttons) {
        textSurface = TTF_RenderText_Blended(ChessQLDfont, button->name.c_str(), button->color);
        textHoveredSurface = TTF_RenderText_Blended(ChessQLDfont, button->name.c_str(), button->hoveredColor);
        if (!textSurface || !textHoveredSurface) {
            fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
        }
        textTextureHovered = SDL_CreateTextureFromSurface(renderer, textHoveredSurface);
        SDL_SetTextureBlendMode(textTextureHovered, SDL_BLENDMODE_BLEND);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
        button->initButton(textTexture, textTextureHovered);
    }
}
/** 
* function to free timer texture because if not done it will cause memory leaks 
* @param timer pointer to timer object
* @return void
*/
void RenderWindow::freeTimer(Timer *timer) {
    //Free texture if it exists
    if (timer->texture != nullptr) {
        SDL_DestroyTexture(timer->texture);
        timer->texture = nullptr;
    }
}

/**
* Function to create a texture from the time of the timer
* @param timer pointer to timer object
* @return void
*/
void RenderWindow::loadFromRenderedText(Timer *timer) {

    // free timer texture before creating a new one
    freeTimer(timer);

    //Render text surface
    if (ChessQLDfont == nullptr) {
        std::cerr << "Font is null" << std::endl;
    }
    // create a surface from the text and texture
    // set the text of the timer and format it to mm:ss
    int time = (int)timer->getTime();
    std::string seconds = std::to_string(time % 60);
    if (seconds.length() == 1) {
        seconds = "0" + seconds;
    }
    timer->timeText = std::to_string(time/60) + ":" + seconds;
    SDL_Surface *textSurface = TTF_RenderText_Blended(ChessQLDfont, timer->timeText.c_str(), timer->textColor);
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


/** function to load texture from file
* @param p_filePath the path to the file
* @return SDL_Texture the texture
*/
SDL_Texture *RenderWindow::loadTexture(const char *p_filePath) {
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == nullptr)
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

    return texture;
}
/** Function to clean up the renderwindow
* @param void
* @return void
*/
void RenderWindow::cleanUp() {

    TTF_CloseFont(ChessQLDfont);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/** Function  to clear the window after each frame
* @param void
* @return void
*/
void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}
/** Default function to render a piece
* @param p_piece the piece to render
* @param whiteDown true if the white player is down
* @return void
*/
void RenderWindow::render(std::shared_ptr <Piece> &p_piece, bool whiteDown) {
    // src is the position and size of the piece on the sprite
    SDL_Rect src;
    src.x = p_piece->getCurrentFrame().x;
    src.y = p_piece->getCurrentFrame().y;
    src.w = p_piece->getCurrentFrame().w;
    src.h = p_piece->getCurrentFrame().h;


    // dst is the position and size of the piece on the board
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
    // so src which is the position and size of the piece on the sprite is copied to dst which is the position and size of the piece on the board
    SDL_RenderCopy(renderer, texture, &src, &dst);
}
/** Function to render Timers and Buttons
* @param buttons an array of buttons
* @param wTimer pointer white timer
* @param bTimer pointer black timer
* @return void
*/
void RenderWindow::renderWidgets(std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer) {
    uint8_t i;
    // render the buttons centered and evenly spaced on the bottom of the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (i = 0; i < buttons.size(); i++) {
        buttons[i]->w = windowx / (2 * (buttons.size() + 2));
        buttons[i]->h = windowy * 0.05;
        buttons[i]->y = windowy * 0.95;
        buttons[i]->x = i * (windowx / (buttons.size() + 2)) + (windowx / (4 * (buttons.size() + 2)));
        // no reference for src since its the whole texture
        // render buttons
        SDL_RenderCopy(renderer, buttons[i]->getTexture(), nullptr, buttons[i]);
    }
    // Could've been done in the loop above but im too lazy to change it
    // Basically ensures the the timers are centeres and evenly spaces as well
    wTimer->w = windowx / (2 * (buttons.size() + 2));
    wTimer->h = windowy * 0.05;
    wTimer->y = windowy * 0.95;
    wTimer->x = i * (windowx / (buttons.size() + 2)) + (windowx / (4 * (buttons.size() + 2)));

    bTimer->w = windowx / (2 * (buttons.size() + 2));
    bTimer->h = windowy * 0.05;
    bTimer->y = windowy * 0.95;
    bTimer->x = (i + 1) * (windowx / (buttons.size() + 2)) + (windowx / (4 * (buttons.size() + 2)));


    // render the timers
    SDL_RenderCopy(renderer, wTimer->texture, nullptr, wTimer);
    SDL_RenderCopy(renderer, bTimer->texture, nullptr, bTimer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


/** Function to render the background
* @param highlight a vector of positions to highlight
* @param lastMoves a vector of positions to highlight as last moves
* @param whiteDown true if the white player is down
* @return void
*/
void RenderWindow::renderbg(const std::vector <glm::ivec2> &highlight = {{1000, 1000}}, const std::vector <glm::ivec2> &lastMoves = {{1000, 1000}}, bool whiteDown = true) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // always update before rendering the background
    updateSquareSize();
    // loop through all 64 squares and render them
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            SDL_Rect rect = {j * squareSize, i * squareSize, squareSize, squareSize};
            // if the sum of the coordinates is even the square is white else its black
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
            // render the last moves and the highlighted squares
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

/** Function to display the rendered frame
* @param void
* @return void
*/
void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}


/**
* Function to render everything
* @param highlight a vector of positions to highlight
* @param lastMoves a vector of positions to highlight as last moves
* @param Pieces a vector containing all pieces
* @param whiteDown true if the white player is down
* @param buttons an array of buttons
* @param wTimer pointer white timer
* @param bTimer pointer black timer
* @return void
*/
void RenderWindow::fullRender(const std::vector <glm::ivec2> &highlight, const std::vector <glm::ivec2> &lastMoves,
                              std::vector <std::shared_ptr<Piece>> &Pieces,
                            std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer, bool whiteDown) {
    // order is important
    // first clear the window
    // then render the background
    // then pieces and widgets
    clear();

    renderbg(highlight, lastMoves, whiteDown);
    loadFromRenderedText(wTimer);
    loadFromRenderedText(bTimer);
    renderWidgets(buttons, wTimer, bTimer);
    for (auto & Piece : Pieces) {
        render(Piece, whiteDown);
    }
}
/** Function to render a text box
* @param textBox the text box to render
* @return void
*/
void RenderWindow::renderTextBox(textBox& textBox) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // make global textfont size smaller for this function
    SDL_Surface* textsurface;
    if (textBox.text.empty()) {
        textBox.text = " "; // so the text box doesnt go invisible
    } 
    SDL_Color textColor = textBox.textcolor;
    if (textBox.text == textBox.placeholder) {
        // placeholder text
        textColor = {161, 167, 179, 255};
    }
    textsurface = TTF_RenderText_Blended_Wrapped(ChessQLDfont, textBox.text.c_str(), textColor, windowx/2);
    // create surface and texture
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



    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderCopy(renderer, texttexture, nullptr, &textrect);

    // free surface and texture
    SDL_FreeSurface(textsurface);
    SDL_DestroyTexture(texttexture);

    // render the cursor not sure if this works lmao
    // blinking not implemented yet
       
   // if (textBox.cursorvisible) {
   //     uint32_t currenttime = SDL_GetTicks();
   //     std::cout << "currenttime" << currenttime << std::endl;
   //     if (currenttime - textBox.lastcursortoggletime >= textBox.cursorblinkrate) {
   //         std::cout << "toggle" << std::endl;
   //         textBox.lastcursortoggletime = currenttime;
   //         std::cout << "lasttime" << textBox.lastcursortoggletime << std::endl;
   //     }

   //     if (textBox.cursorvisible) {
            int cursorx = textBox.x + textrect.w + 2; // adjust the cursor position as needed
            SDL_RenderDrawLine(renderer, cursorx, textBox.y + 2, cursorx, textBox.y + textrect.h - 2);
   //     }
   // }
    // since its a new loop we need to manually display the changes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


/**
* Funtion to show and handle the text box
* @param textBox the text box to render
* @return std::string the text entered
*/
std::string RenderWindow::TextBox(textBox textBox, std::vector <std::shared_ptr<Piece>> &Pieces,
                              std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer) {

    TTF_SetFontSize(ChessQLDfont, 32);
    // new game loops just for textbox because implementing it in the other loop would be a pain
    while (!textBox.isDone) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // If the user closes the window, exit the loop and the function
                textBox.isDone = true;
                TTF_SetFontSize(ChessQLDfont, 128);
                return "close";
            }
            textBox.handleEvent(event);
        }
        fullRender({{1000, 1000}}, {{1000,1000}}, Pieces, buttons, wTimer, bTimer);
        renderTextBox(textBox);
        SDL_RenderPresent(renderer);
    }
    TTF_SetFontSize(ChessQLDfont, 128);
    return textBox.text;
}

/**
* Function to display the welcome message
* @param text the text to display
* @return bool true if the user closed the window
*/
bool RenderWindow::displayMessage(std::string text) {
    SDL_Rect textRect;
    SDL_Texture *textTexture;
    std::string welcomeText = std::move(text);
    SDL_Color textColor = {255, 0, 0};
    SDL_Surface *textSurface = TTF_RenderText_Blended(ChessQLDfont, welcomeText.c_str(), textColor);
    if (!textSurface) {
        fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
        return true;
    }

    // Create a texture from the rendered text surface and set its blend mode to alpha blending
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

    // Calculate the position of the text so it is centered on the screen

    // Set the flag to false initially
    bool playButtonPressed = false;

    // Loop until the "string" button is pressed
    while (!playButtonPressed) {
        updateSquareSize();
        textRect.w = windowx / 1.01;
        textRect.h = windowy / 5;
        textRect.x = (windowx - textRect.w) / 2;
        textRect.y = (windowy - textRect.h) / 2;
        // dst
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_RenderPresent(renderer);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // If the user closes the window, exit the loop and the function
                playButtonPressed = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
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
    }

    // Destroy the texture and free the surface after the loop
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    return true;
}
/** Function to display the promotion options
* 1 = queen, 2 = rook, 3 = bishop, 4 = knight
* @param pos the position of the pawn
* @param white true if the pawn is white
* @return int which represents that the piece to promotes to
*/
int RenderWindow::displayPromotionOptions(glm::vec2 pos, bool white) {
    // create a box that is evenly spaced and display all pieces depending of is bool is set to white or not white or black
    SDL_SetRenderDrawColor(renderer, 166, 168, 171, 200);
    int calculations = (int) pos.y * squareSize - (white ? 0 : 3 * squareSize);
    SDL_Rect rect = {(int) pos.x * squareSize, calculations, (squareSize), (squareSize * 4)};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect outline = {(int) pos.x * squareSize, calculations, (squareSize), (squareSize * 4)};
    SDL_RenderDrawRect(renderer, &outline); 

    // render all the pieces on the sprite this is ugly but it gets the job done
    for (int i = 128; i <= 128 * 4; i += 128) {
        SDL_Rect src;
        src.h = 128;
        src.w = 128;
        if (white)
            src.y = 128;
        else 
            src.y = 0;

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
