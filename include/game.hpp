#include <glm/fwd.hpp>
#include <memory>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include "renderWindow.hpp"
#include "piece.hpp"
#include "util.hpp"
class Game
{
private:
    // SDL specific stuff
    SDL_Event event;

    //Game Logic
    bool gameRunning = true;
    bool isPieceSelected = false;
    bool whiteTurn;
    bool whiteDown = false; // defines which color is on the bottom of the board when game is startes
    bool rotate_board = false; // defines if the board should rotate after every move or not
    bool isPromoting = false;
//    int halfMoveNumber = 0;
    int fullMoveNumber = 0;

    // RenderWindow stuff
    RenderWindow window;
    std::vector<glm::vec2> lastPositions = {{1000,1000}};
    std::vector<std::shared_ptr<Piece>> lastPieces;

    // Pieces 
    std::shared_ptr<Piece> selectedPiece;
    std::vector<std::shared_ptr<Piece>> Pieces;

    void run();

    std::vector<std::shared_ptr<Piece>> FenImport(std::string FenString);
    void handleEvents();
    void handleCheckmate();
    bool handleProtomotion(std::shared_ptr<Piece> selectedPiece);
    void selectPiece();
    void handlePromotionPieceSelection(glm::vec2 selection);
    void DragPiece();
    void placePiece();


public:
    Game(std::string fen); 
    ~Game();

};
