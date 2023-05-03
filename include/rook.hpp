#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Entity.hpp"
class Rook: public Entity
{
public:
    Rook(glm::vec2 p_pos, bool white);
    void test() override {
        std::cout<<"this sisr r"<<std::endl;
    }
private:
};
