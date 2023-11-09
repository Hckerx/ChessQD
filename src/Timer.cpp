#include "timer.hpp"
#include "SDL2/SDL.h"

Timer::Timer() {
    startTicks = 0;
    pausedTicks = 0;
    isPaused = false;
    isStarted = false;
}

void Timer::start() {
    isStarted = true;
    isPaused = false;
    startTicks = SDL_GetTicks();
}

void Timer::pause(){
    isStarted = true;
    pausedTime = getTicks(); 
    isPaused = !isPaused;
    if (!isPaused){
        pausedTicks += (getTicks() - pausedTime);
    }
}

void Timer::stop() {
    isStarted = false;
    isPaused = false;
}

void Timer::reset() {
    startTicks = 0;
    pausedTicks = 0;
    isPaused = false;
    isStarted = false;
}


Uint32 Timer::getTicks() const {
    if (isStarted) {
        if (isPaused)
        {
            return pausedTime;
        }
        else
        {
          return SDL_GetTicks() - startTicks - pausedTicks;
        }  
        
    }
    return 0;
}
