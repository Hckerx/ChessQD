#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "entity.hpp"
#include <iostream>
class Pawn: public Entity
{
public:
    Pawn(glm::vec2 p_pos, bool white);
    void findMoves(); 

    bool move(glm::vec2 newPos, glm::vec2 oldPos) override;
private:
    bool hasMoved = false;
};
