#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <sstream>

class Timer : public SDL_Rect {
public:
    Timer();

    void startPause();

    void stop();

    void reset();

    Uint32 getTicks() const;

    bool isPaused = true;
    SDL_Color textColor = {0, 0, 0, 255};
    std::string timeText = "00:00";
    SDL_Texture *texture = NULL;

private:
    void free();

    Uint32 startTicks = 0;
    Uint32 pausedTicks = 0;
    Uint32 pausedTime = 0;
};