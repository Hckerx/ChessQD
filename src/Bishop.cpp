#include "bishop.hpp"
#include "Entity.hpp"
#include "SDL2/SDL.h"


#include "glm/glm.hpp"
#include <iostream>
Bishop::Bishop(glm::vec2 p_pos, SDL_Texture* p_tex, bool white)
:Entity(p_pos, p_tex){
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 128*2;
        currentFrame.w = 128;
        currentFrame.h = 128;
}
