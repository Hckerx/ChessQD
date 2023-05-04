#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Entity.hpp"
class King: public Entity
{
public:
    King(glm::vec2 p_pos, bool white);
    void highlightPiece() {
            std::cout<<"this sisr r"<<std::endl;
        	}
private:
};

