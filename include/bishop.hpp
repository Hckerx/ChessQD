#include <SDL2/SDL.h>
#include <glm/glm.hpp>


#include "Entity.hpp"
class Bishop: public Entity
{
public:
    Bishop(glm::vec2 p_pos, SDL_Texture* p_tex, bool white);

private:
};
