#include <glm/fwd.hpp>
#include <memory>
#define SDL_MAIN_HANDLED
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

    //Game Logic
    bool gameRunning = true;
    bool isPieceSelected;
    bool white_turn = true;
    bool playing_white = true;
    bool rotate_board = true; // set to playing white if you want behaviour of rotating board on move
    bool game_started = false;
    bool hasClickedPiece = false;
    bool isOverridable = false;

    // RenderWindow stuff
    RenderWindow window;
    std::vector<glm::vec2> lastPositions = {{1000,1000}};
    std::vector<std::shared_ptr<Entity>> lastEntities;
    int counter = 0;

    // Pieces 
    std::vector<std::shared_ptr<Entity>> Pieces = FenImport("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    std::shared_ptr<Entity> selectedEntity;


    void run();
    void handleEvents();
    void handleCheckmate();
    void selectPiece();
    void DragPiece();
    void placePiece();
public:
    Game(/* args */); 
    ~Game();

};
