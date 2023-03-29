#include <SDL2/SDL.h>
#include <glm/glm.hpp>


#include "Entity.hpp"
class Queen: public Entity
{
public:
    Queen(glm::vec2 p_pos, SDL_Texture* p_tex, bool white);

private:
};
