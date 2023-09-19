#define SDL_MAIN_HANDLED

#include <memory>
#include <vector>
#include "renderWindow.hpp"
#include "piece.hpp"
#include "chessCommunication.hpp"
class Game
{
private:
    // SDL specific stuff
    SDL_Event event;

    // Communication
    std::unique_ptr<Communication> communication = nullptr;

    // threading
    std::future<std::string> futurerecv;
    //Game Logic

    bool gameRunning = true;
    bool PieceSelected = false; //Does user hold leftclick on piece
    bool whiteTurn = true;
    bool whiteDown = true; // defines which color is on the bottom of the board when game is startes
    bool rotate_board = false; // defines if the board should rotate after every move or not
    bool isPromoting = false; //needed??????
    int halfMoveNumber = 0;
    int fullMoveNumber = 0; //needed?
    int state = 5; //state of the game (draw, checkmate, closed)
    int counter = 0; //needed?
    bool isServer; //bullshit
    bool isPlayingOnline = false; //maybe other place?
    bool isWhite = false; //DUUMMMM!!!

    // RenderWindow stuff
    RenderWindow window; //displayed main window
    std::vector<glm::ivec2> highlightMoves = {{1000,1000}}; //vector of moves to highlight
    std::vector<glm::ivec2> lastMoves = {{1000, 1000}}; //vector of last moves??

    std::shared_ptr<Piece> lastPiece;

    // Pieces 
    std::shared_ptr<Piece> selectedPiece;
    std::vector<std::shared_ptr<Piece>> Pieces;
    std::vector<std::string> moveHistory;

    void run();

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
