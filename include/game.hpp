#include <glm/fwd.hpp>
#include <memory>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include "renderWindow.hpp"
#include "piece.hpp"
#include "util.hpp"
    //Game Logic
    bool gameRunning = true;
    bool isPieceSelected = false;
    bool whiteTurn = true;
    bool whiteDown = false; // defines which color is on the bottom of the board when game is startes
    bool rotate_board = false; // defines if the board should rotate after every move or not

    // RenderWindow stuff
    RenderWindow window;
    std::vector<glm::vec2> lastPositions = {{1000,1000}};
    std::vector<std::shared_ptr<Piece>> lastPieces;
    int counter = 0;

    // Pieces 
    std::vector<std::shared_ptr<Piece>> Pieces = FenImport("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    std::shared_ptr<Piece> selectedPiece;


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
