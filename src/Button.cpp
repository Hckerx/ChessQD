#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "button.hpp"
#include <SDL2/SDL.h>
Button::Button(std::string name, TTF_Font* font)
:SDL_Rect(), name(name){

}
void Button::initButton(TTF_Font* font, SDL_Renderer* renderer){
    SDL_Surface* textSurface;
    SDL_Surface* textHoveredSurface;
    textSurface = TTF_RenderText_Blended(font, name.c_str(), color);
    textHoveredSurface = TTF_RenderText_Blended(font, name.c_str(), hoveredColor);
    if (!textSurface || !textHoveredSurface) {
        fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
    }

    textTextureHovered = SDL_CreateTextureFromSurface(renderer, textHoveredSurface);
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
}
SDL_Texture* Button::getTexture(){

    if (hovered()){
        return textTextureHovered;
    }
    return textTexture;
}
bool Button::hovered() {
        
        SDL_GetMouseState(&Mouse_x,&Mouse_y);
        point = {Mouse_x,Mouse_y};
        return SDL_PointInRect(&point,this);
}
