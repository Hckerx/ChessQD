#include <string>
#include <utility>
#include "button.hpp"

/** A button class inheriting from the SDL_Rect object.
* It has a name and a texture and can be hovered and clicked.
* @param name the name of the button
*/
Button::Button(std::string name)
        : SDL_Rect(), name(std::move(name)) {
} //HELP FIXME

// initialize with its textures not in constructor because needs renderer
void Button::initButton(SDL_Texture *textTexture, SDL_Texture *textHoveredTexture) {
    Texture = textTexture;
    TextureHovered = textHoveredTexture;
}
/**
* getter for the texture of the button depending on if it is hovered or not
* @param void
* @return the texture of the button
*/
SDL_Texture * Button::getTexture() {
    if (hovered())
        return TextureHovered;
    return Texture;
}

/** checks if the mouse is over the button
* @param void
* @return true if the mouse is over the button
*/
bool Button::hovered() {
    int Mouse_x, Mouse_y;
    SDL_Point point;
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    point = {Mouse_x, Mouse_y};
    return SDL_PointInRect(&point, this);
}
