#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "button.hpp"
#include <SDL2/SDL.h>
Button::Button(std::string name, TTF_Font* font)
:SDL_Rect(), ChessQLDfont(font) ,name(name){

    textSurface = TTF_RenderText_Blended(ChessQLDfont, name.c_str(), color);
    textHoveredSurface = TTF_RenderText_Blended(ChessQLDfont, name.c_str(), hoveredColor);
    if (!textSurface || !textHoveredSurface) {
        fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
    }
}
SDL_Surface* Button::getSurface(){
    if (hovered()){
        return textHoveredSurface;
    }
    return textSurface;
}
bool Button::hovered() {
        
        SDL_GetMouseState(&Mouse_x,&Mouse_y);
        point = {Mouse_x,Mouse_y};
        return SDL_PointInRect(&point,this);
}
