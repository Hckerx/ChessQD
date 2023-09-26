#include <SDL2/SDL_rect.h>
#include <string>
class Button : public SDL_Rect
{
public:
    Button(int x,int y,int w,int h,std::string name);
    bool clicked();
    bool hovered();
    std::string name;
private:



};