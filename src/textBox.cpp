//
// Created by daniel on 02.12.23.
//
#include "textBox.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <string>
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

std::string textBox::gettext() const{
    return text;
}



