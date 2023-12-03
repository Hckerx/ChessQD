#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <string>

class Timer : public SDL_Rect {
public:
    Timer();

    void startPause();

    void stop();

    void reset();

    [[nodiscard]] float getTime() const;

    bool isPaused = true;
    SDL_Color textColor = {0, 0, 0, 255};
    std::string timeText = "00:00";
    SDL_Texture *texture = nullptr;

private:
    void free();

    uint32_t startTicks = 0;
    uint32_t pausedTicks = 0;
    uint32_t pausedTime = 0;
};