#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "entity.hpp"
class Knight: public Entity
{
public:
    Knight(glm::vec2 p_pos, bool white);
    void findMoves();

private:
};
