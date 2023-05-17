#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "entity.hpp"
class Rook: public Entity
{
public:
    Rook(glm::vec2 p_pos, bool white);
    void findMoves(std::vector<std::shared_ptr<Entity>>& Pieces) ;
;private:
};
