#pragma once

#include <SDL2/SDL_rect.h>
#include <array>
#include <glm/fwd.hpp>

#define SDL_MAIN_HANDLED

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "button.hpp"
#include "piece.hpp"
#include "timer.hpp"
#include "textBox.hpp"


class RenderWindow {
public:
    explicit RenderWindow(const char *p_title);

    void renderTextBox(textBox& textBox);
    bool displayWelcomeMessage(std::string text);

    void cleanUp();

    void fullRender(const std::vector <glm::ivec2> &highlight, const std::vector <glm::ivec2> &lastMoves,
                              std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteDown,
                              std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer);

    int displayPromotionOptions(glm::vec2 pos, bool white);

    int squareSize;

    std::string TextBox(textBox textBox);
    void updateSquareSize() {
        SDL_GetWindowSize(window, &windowx, &windowy);
        squareSize = std::min(windowx, (int) (windowy * 0.95)) / 8;
    }

    void display();

    int windowx;
    int windowy;
    void initButtons(std::array<Button *, 3> buttons);

private:

    void loadFromRenderedText(Timer *timer);

    int renderWidgets(std::array<Button *, 3> buttons, Timer *wTimer, Timer *bTimer);

    SDL_Texture *texture;
    SDL_Window *window;
    SDL_Renderer *renderer;

    void freeTimer(Timer *timer);

    TTF_Font *ChessQLDfont;

    void clear();

    void render(std::shared_ptr <Piece> &p_piece, bool whiteDown);

    void renderbg(const std::vector <glm::ivec2> &highlight, const std::vector <glm::ivec2> &lastMoves, bool whiteDown);

    SDL_Texture *loadTexture(const char *p_filePath);
};
