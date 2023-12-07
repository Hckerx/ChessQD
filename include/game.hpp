#pragma once
#include <sys/types.h>
#include <array>
#include <memory>
#include <vector>
#include "renderWindow.hpp"
#include "communication.hpp"

class Game {

private:
    // SDL Input events
    SDL_Event event;

    // Online logic
    Communication *communication = nullptr;

    boost::asio::io_context io_context;

    // Game Logic
    bool isPlayingOnline = false;
    bool isWhite() {
        if (communication != nullptr) {
            return communication->isWhite;
        }
        return false;
    }

    // Render class to render game
    RenderWindow window; //displayed main window
    std::vector <glm::ivec2> highlightMoves = {{1000, 1000}};
    std::vector <glm::ivec2> lastMoves = {{1000, 1000}}; 
    std::shared_ptr <Piece> lastPiece; // used for promoting 
    // Widgets
    Timer wTimer;
    Timer bTimer;
    std::array<Button *, 3> buttons;

    // Game Logic
    bool gameRunning = true;
    bool PieceSelected = false; //Does user hold leftclick on piece
    bool whiteTurn = true;
    bool whiteDown = true; // defines which color is on the bottom of the board when game is started
    bool rotate_board = false; // defines if the board should rotate after every move or not
    bool isPromoting = false;
    int halfMoveNumber = 0;
    int fullMoveNumber = 0; 
    int state = -1; //state = -1 -> game is running, state = 0 -> white lost, state = 1 -> black lost, state = 2 -> draw, state = 3 -> quit
    u_long counter = 0; //move forwards and backwards



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
