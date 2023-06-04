#include <glm/glm.hpp>
#include <iostream>

#include "piece.hpp"
class Bishop: public Piece
{
public:
    Bishop(glm::vec2 p_pos, bool white);
    void findMoves(std::vector<std::shared_ptr<Piece>>& Pieces); 
private:
};
