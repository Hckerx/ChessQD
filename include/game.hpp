#include <sys/types.h>
#define SDL_MAIN_HANDLED
#include <array>
#include <memory>
#include <vector>
#include "renderWindow.hpp"
#include "piece.hpp"
#include "communication.hpp"
#include <iostream>

class Game {

private:
    // SDL specific stuff
    SDL_Event event;

    // Communication
    //std::unique_ptr<Communication> communication = nullptr;
    Communication *communication = nullptr;

    boost::asio::io_context io_context;
    //Game Logic

    bool isPlayingOnline = false;
    bool isWhite() {
        if (communication != nullptr) {
            return communication->isWhite;
        }
        return false;
    }


    Timer wTimer;
    Timer bTimer;
    bool gameRunning = true;
    bool PieceSelected = false; //Does user hold leftclick on piece
    bool whiteTurn = true;
    bool whiteDown = true; // defines which color is on the bottom of the board when game is startes
    bool rotate_board = false; // defines if the board should rotate after every move or not
    bool isPromoting = false; //needed??????
    int halfMoveNumber = 0;
    int fullMoveNumber = 0; //needed?
    int state = -1; //state of the game (draw, checkmate, closed)
    u_long counter = 0; //move forwards and backwards

    std::array<Button *, 3> buttons;

    // RenderWindow stuff
    RenderWindow window; //displayed main window
    std::vector <glm::ivec2> highlightMoves = {{1000, 1000}}; //vector of moves to highlight
    std::vector <glm::ivec2> lastMoves = {{1000, 1000}}; //vector of last moves?? CHANGE NAME

    std::shared_ptr <Piece> lastPiece; //FIXME WHATS THTA NAME

    // Pieces 
    std::shared_ptr <Piece> selectedPiece;
    std::vector <std::shared_ptr<Piece>> Pieces;
    std::vector <std::string> moveHistory;

    void run();
   
    void handleEvents();
    void handleCheckmate();
    bool handlePromotion(const std::shared_ptr<Piece> &selectedPiece, bool Captured);
    void selectPiece();
    void handlePromotionPieceSelection(glm::vec2 selection);
    void DragPiece();
    void placePiece();
    std::vector <std::shared_ptr<Piece>> FenImport(const std::string &FenString);

public:

    explicit Game(std::string fen);
    ~Game();
};
