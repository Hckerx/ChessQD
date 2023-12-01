#include "piece.hpp"

class Knight : public Piece {
public:
    Knight(glm::vec2 p_pos, bool white);

    void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces);

private:
};
