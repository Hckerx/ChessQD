#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "entity.hpp"
class Rook: public Entity
{
public:
    Rook(glm::vec2 p_pos, bool white);
    void findMoves(std::vector<Entity*> Pieces) ;
;private:
    bool findIndMoves(std::vector<Entity *> Pieces,int x,int y);
};
