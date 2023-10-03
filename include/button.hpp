#include <SDL2/SDL_rect.h>
#include <string>
#include <array>
class Button : public SDL_Rect
{
public:
    Button(std::string name);
    bool clicked();
    SDL_Color getColor();
    void updateRect();
    int Mouse_x, Mouse_y;
    SDL_Point point;
    bool hovered();
    std::string name;
private:
    SDL_Color color = {255, 255, 255};
    SDL_Color hoveredColor = {255, 0, 0};
};