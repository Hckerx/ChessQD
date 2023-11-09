#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <array>
class Button : public SDL_Rect
{
public:
    Button(std::string name, TTF_Font* font);
    TTF_Font* ChessQLDfont;
    bool clicked();
    SDL_Surface* getSurface();
    void updateRect();
    int Mouse_x, Mouse_y;
    SDL_Point point;
    bool hovered();
    std::string name;
private:
    SDL_Surface* textSurface;
    SDL_Surface* textHoveredSurface;
    SDL_Color color = {255, 255, 255};
    SDL_Color hoveredColor = {255, 0, 0};
};
