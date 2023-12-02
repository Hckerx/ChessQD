#include "timer.hpp"
#include <SDL2/SDL_timer.h>

Timer::Timer() : SDL_Rect() {
}

void Timer::startPause() {
    if (!isPaused) {
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    } else {
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
    isPaused=!isPaused;
}

void Timer::stop() {
    isPaused = false;
    startTicks = 0;
    pausedTicks = 0;
}
//returns current timer time
float Timer::getTime() const {
    if (isPaused) {
        return (float)pausedTicks / 1000.f;
    } else {
        return (float)(SDL_GetTicks() - startTicks) / 1000.f;
    }
}
