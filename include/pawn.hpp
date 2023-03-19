#include <SDL2/SDL.h>
#include <glm/glm.hpp>


#include "Entity.hpp"
class Pawn: public Entity
{
public:
    Pawn(glm::vec2 p_pos, SDL_Texture* p_tex, bool white);

private:
};
