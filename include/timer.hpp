#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <sstream>

class Timer : public SDL_Rect{
public:
    Timer();
    void start();
    void pause();
    void stop();
    void reset();
    Uint32 getTicks() const;
    bool isPaused;
    bool isStarted;
    SDL_Color textColor = { 0, 0, 0, 255 };
    std::stringstream timeText;
    SDL_Texture* texture = NULL;

private:
    void free();
    Uint32 startTicks;
    Uint32 pausedTicks;
    Uint32 pausedTime;
};
