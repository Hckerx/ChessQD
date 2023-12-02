#pragma once //???
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Piece {
public:
    virtual ~Piece() = default;

    Piece(glm::vec2 p_pos, bool white);

    glm::vec2 &getPos() {
        return pos;
    }

    [[nodiscard]] glm::vec2 getPosCopy() const {
        return pos;
    }

    void setPos(glm::vec2 posold) {
        pos.x = posold.x;
        pos.y = posold.y;
    }

    virtual void setParameters() {};

    virtual bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
                      bool isPlayingOnline, bool isWhite);

    bool findIndMoves(std::vector <std::shared_ptr<Piece>> &Pieces, float x, float y);

    virtual void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) = 0;

    bool isKingInCheck(std::vector <std::shared_ptr<Piece>> &Pieces) const;

    bool findMoves(std::vector <std::shared_ptr<Piece>> &Pieces);

    std::vector <glm::vec2> legalMoves;

    [[nodiscard]] SDL_Rect getCurrentFrame() const;

    SDL_Rect currentFrame;
    glm::vec2 pos;
    bool white;
};
