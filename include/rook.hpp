#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "piece.hpp"
class Rook: public Piece
{
public:
    Rook(glm::vec2 p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces) ;
;private:
};
