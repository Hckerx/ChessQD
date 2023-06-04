#include <glm/glm.hpp>
#include "piece.hpp"
#include <iostream>
class Pawn: public Piece
{
public:
    Pawn(glm::vec2 p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces);
    bool isEnPassantVulnerable;
private:
};
