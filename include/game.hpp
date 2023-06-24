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
    bool whiteDown = true; // defines which color is on the bottom of the board when game is startes
    bool rotate_board = false; // defines if the board should rotate after every move or not
    bool isPromoting = false;
    int halfMoveNumber = 0;
    int fullMoveNumber = 0;
    bool draw = false;

    // RenderWindow stuff
    RenderWindow window;
    std::vector<glm::ivec2> highlightMoves = {{1000,1000}};
    std::vector<glm::ivec2> lastMoves = {{1000, 1000}};

    std::vector<std::shared_ptr<Piece>> lastPieces;

    // Pieces 
    std::shared_ptr<Piece> selectedPiece;
    std::vector<std::shared_ptr<Piece>> Pieces;
    std::vector<std::string> moveHistory;

    bool run();

    std::vector<std::shared_ptr<Piece>> FenImport(std::string FenString);
    std::string FenExport(std::vector<std::shared_ptr<Piece>> piecesVector);
    void handleEvents();
    void handleCheckmate();
    bool handleProtomotion(std::shared_ptr<Piece> selectedPiece, bool Captured);
    void selectPiece();
    void handlePromotionPieceSelection(glm::vec2 selection);
    void DragPiece();
    void placePiece();

public:
    Game(std::string fen); 
    ~Game();

};
