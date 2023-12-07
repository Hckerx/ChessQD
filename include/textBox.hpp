#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <string>
#include <utility>
#include <sys/types.h>
class textBox {
public:
    textBox(std::string placeholder, int x, int y) :x(x), y(y), placeholder(std::move(placeholder)) {
        text = placeholder;
        cursorvisible = true;
        cursorblinkrate = 500; // Cursor blink rate in milliseconds
        lastcursortoggletime = SDL_GetTicks();
    }

    void handleEvent(SDL_Event &event);
    [[nodiscard]] std::string gettext() const;

    int x, y;
    bool isDone = false;
    const std::string placeholder;
    std::string text;
    SDL_Color textcolor = {0, 0, 0, 255};
    bool cursorvisible;
    uint32_t cursorblinkrate;
    uint32_t lastcursortoggletime;
};

