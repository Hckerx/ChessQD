#pragma once
#include "piece.hpp"

class Rook : public Piece {
public:
    Rook(glm::vec2 p_pos, bool white);

    void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) override;

    bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
              bool isPlayingOnline, bool isWhite) override;

    bool hasMoved = true;;
};
