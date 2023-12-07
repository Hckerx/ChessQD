#include "textBox.hpp"
#include <string>

/** Function to handly key presses and text input
* @param event SDL_Event needed to handle key presses
* @return void
*/
void textBox::handleEvent(SDL_Event &event) {
    if (event.type == SDL_TEXTINPUT) {
        // remve placeholder when typing
        if (text == placeholder)
            text = "";
        text += event.text.text;
    } else if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            text.pop_back();
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            if (text == placeholder)
                text = "default";
            // do something with the text

            
            isDone = true;
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
