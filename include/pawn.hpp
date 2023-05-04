#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Entity.hpp"
#include <iostream>
class Pawn: public Entity
{
public:
    void findMoves() {
        std::cout<<"this sisr r"<<std::endl;
    }
    Pawn(glm::vec2 p_pos, bool white);

private:
};
