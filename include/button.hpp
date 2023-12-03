#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <array>

class Button : public SDL_Rect {
public:
    explicit Button(std::string name);

    bool clicked();

    SDL_Surface *getSurface();

    void updateRect();


    bool hovered();

    std::string name;

    void initButton(SDL_Texture *textTexture, SDL_Texture *textHoveredTexture);

    SDL_Texture *getTexture();

    SDL_Color color = {0, 0, 0};
    SDL_Color hoveredColor = {255, 0, 0};
private:
    SDL_Texture *Texture;
    SDL_Texture *TextureHovered;

};
