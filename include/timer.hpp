#include <SDL2/SDL_rect.h>

class Timer : public SDL_Rect{
public:
    Timer();
    void start();
    void pause();
    void stop();
    void reset();
    Uint32 getTicks() const;

private:
    Uint32 startTicks;
    Uint32 pausedTicks;
    Uint32 pausedTime;
    bool isPaused;
    bool isStarted;
};