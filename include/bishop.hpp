#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "entity.hpp"
class Bishop: public Entity
{
public:
    Bishop(glm::vec2 p_pos, bool white);
    void findMoves(std::vector<Entity*> Pieces); 
private:
};
