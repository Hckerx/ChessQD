#include "piece.hpp"
class Pawn: public Piece
{
public:
    Pawn(glm::vec2 p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces);
    bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>> &Pieces, bool whiteTurn);
    bool isEnPassantVulnerable = false;

private:
};
