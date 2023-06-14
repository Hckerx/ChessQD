#include <glm/glm.hpp>
#include <iostream>

#include "piece.hpp"
class King: public Piece
{
public:
    King(glm::vec2 p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces); 
    bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn);
    bool hasMoved = false;
private:
};

