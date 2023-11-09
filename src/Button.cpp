#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "button.hpp"
#include <SDL2/SDL.h>
Button::Button(std::string name)
:SDL_Rect(), name(name){

}
void Button::initButton(SDL_Texture* textTexture, SDL_Texture* textHoveredTexture){

Texture = textTexture;
TextureHovered = textHoveredTexture;
    
}
SDL_Texture* Button::getTexture(){

    if (hovered()){
        return TextureHovered;
    }
    return Texture;
}
bool Button::hovered() {
        
        SDL_GetMouseState(&Mouse_x,&Mouse_y);
        point = {Mouse_x,Mouse_y};
        return SDL_PointInRect(&point,this);
}
