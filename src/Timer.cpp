#include "timer.hpp"
#include <SDL2/SDL_timer.h>
/**
* Timer constructor inheriting from SDL_Rect
 */
Timer::Timer() : SDL_Rect() {
}

/** Function which is responsible for the logic of the timer
* @param none
* @return void
*/
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

/** Function to stop the timer
* @param none
* @return void
*/
void Timer::stop() {
    isPaused = true;
    startTicks = 0;
    pausedTicks = 0;
}
/** Function which returns the time
* @param none
* @return Time in ms float
*/
float Timer::getTime() const {
    if (isPaused) {
        return (float)pausedTicks / 1000.f;
    } else {
        return (float)(SDL_GetTicks() - startTicks) / 1000.f;
    }
}
