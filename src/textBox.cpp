#include "textBox.hpp"
#include <string>

/** Function to handly key presses and text input
* @param event SDL_Event needed to handle key presses
* @return void
*/
void textBox::handleEvent(SDL_Event &event) {
    if (event.type == SDL_TEXTINPUT) {
        text += event.text.text;
    } else if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            text.pop_back();
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            // do something with the text
            isDone = true;
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
