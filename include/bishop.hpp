#include "piece.hpp"

class Bishop : public Piece {
public:
    Bishop(glm::vec2 p_pos, bool white);

    // moves the piece to a new position
    void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) override;
};
