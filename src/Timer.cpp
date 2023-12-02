#include "timer.hpp"
#include <iostream>
#include "SDL2/SDL.h"

Timer::Timer() : SDL_Rect() {
}

void Timer::startPause() {
    if (!isPaused) {
        isPaused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    } else {
        std::cout << "pause" << std::endl;
        isPaused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

void Timer::stop() {
    isPaused = false;
    startTicks = 0;
    pausedTicks = 0;
}

Uint32 Timer::getTicks() const {
    if (isPaused) {
        return pausedTicks;
    } else {
        return SDL_GetTicks() - startTicks;
    }
}
