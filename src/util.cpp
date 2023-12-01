#include "util.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "piece.hpp"

std::shared_ptr <Piece> getMatchingPiece(glm::vec2 field, std::vector <std::shared_ptr<Piece>> &Pieces) {

    for (auto &i: Pieces) {
        if (field == i->pos) {
            return i;
        }
    }
    return nullptr;
}

glm::vec2 getMousePosition(bool whiteDown, int squareSize) {
    int Mouse_x, Mouse_y;
    float Mousex, Mousey;
    SDL_GetMouseState(&Mouse_x, &Mouse_y);


    if (whiteDown) {
        Mousex = (float) Mouse_x / (float) squareSize;
        Mousey = (float) Mouse_y / (float) squareSize;
        Mouse_x = Mouse_x / squareSize;
        Mouse_y = Mouse_y / squareSize;

    } else {
        Mousex = 8 - (float) Mouse_x / (float) squareSize;
        Mousey = 8 - (float) Mouse_y / (float) squareSize;
        Mouse_x = 8 - Mouse_x / squareSize;
        Mouse_y = 8 - Mouse_y / squareSize;
    }
    return glm::vec2{Mousex, Mousey};
}

