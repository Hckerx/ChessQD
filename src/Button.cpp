#include <SDL2/SDL_rect.h>
#include <iostream>
#include <string>
#include "button.hpp"
#include <SDL2/SDL.h>
Button::Button(std::string name)
:SDL_Rect(), name(name){
}
SDL_Color Button::getColor(){
    if (hovered()){
        return hoveredColor; 
    }
    return color;
}
bool Button::hovered() {
        SDL_GetMouseState(&Mouse_x,&Mouse_y);
        point = {Mouse_x,Mouse_y};
        return SDL_PointInRect(&point,this);
}
