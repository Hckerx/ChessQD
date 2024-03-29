#pragma once
#include "piece.hpp"

class Queen : public Piece {
public:
    Queen(glm::vec2 p_pos, bool white);

    void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) override;
};
