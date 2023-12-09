#include "textBox.hpp"
#include <SDL2/SDL_clipboard.h>
#include <string>
#include <iostream>

/** Function to handly key presses and text input
* @param event SDL_Event needed to handle key presses
* @return void
*/
void textBox::handleEvent(SDL_Event &event) {
    if (event.type == SDL_TEXTINPUT) {
        // remve placeholder when typing
        if (text == placeholder) {
            text = "";
        }
        text += event.text.text;
    } else if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            if (text == placeholder) {
                text = "";
            } else {
                text.pop_back();
            }
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            if (text == placeholder) {
                text = "default";
            }
            isDone = true;
        }
        // check for control V
        else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
            // get clipboard text
            char *clipboardText = SDL_GetClipboardText();
            // remove placeholder when typing
            if (text == placeholder) {
                text = "";
            }
            text += clipboardText;
            SDL_free(clipboardText);
        }
            
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        // if mouse is clicked outside of the box, remove the box
        if (event.button.x < x || event.button.x > x + 200 || event.button.y < y || event.button.y > y + 50) {
            isDone = true;
            text = "no";
        } 
    }
}

/** Function that gets the text from the textBox
* @param none
* @return std::string
*/
std::string textBox::gettext() const{
    return text;
}
