#include <glm/glm.hpp>
#include "entity.hpp"
#include <iostream>
class Pawn: public Entity
{
public:
    Pawn(glm::vec2 p_pos, bool white);
    void findMoves(std::vector<std::shared_ptr<Entity>>& Pieces);
private:
    glm::vec2 lastPos;
};
