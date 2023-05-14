#include <SDL2/SDL.h>
#include <vector>
#include "renderWindow.hpp"
#include "entity.hpp"
#include "util.hpp"
class Game
{
private:
    // SDL specific stuff
    SDL_Event event;

    // Inputs
    int Mouse_x, Mouse_y;

    //Game Logic
    bool gameRunning = true;
    bool isPieceSelected;
    bool playing_white = true;

    // RenderWindow stuff
    RenderWindow window;
    std::vector<glm::vec2> lastPositions = {{1000,1000}};

    // Pieces 
    std::vector<Entity*> Pieces = FenImport("8/8/8/8/R7/8/8/8");
    Entity* selectedEntity;

    void run();
    void handleEvents();
    void selectPiece();
    void DragPiece();
    void placePiece();
public:
    Game(/* args */);
    ~Game();

};
