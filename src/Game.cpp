// import libraries
#include <glm/gtx/string_cast.hpp>
#include <cstdint>
#include <memory>
#include <bits/stdc++.h>

// import files
#include "renderWindow.hpp"
#include "util.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "game.hpp"
// define constants
#define RESIGN 1
#define ONLINE 0
// TODO: Threefold FIVEFOLD seventy move rule insufficient material?

/** constructor of class Game (the main class)
* @param void
* @return void
*/
Game::Game(std::string fen) : window("ChessQLD") {
    //window.displayMessage("Welcome to ChessQLD");
    // import default fen
    Pieces = FenImport(fen);
    moveHistory.push_back(fen);
    // create Timers and Buttons
    wTimer = Timer();
    bTimer = Timer();
    buttons = {new Button("resign"), new Button("online"), new Button("rotate")};
    window.initButtons(buttons);
    // start Game
    run();
}

/** Destructor of class Game
* @param void
* @return void
*/
Game::~Game() {
    window.cleanUp();
}

/**
* Game loop
* @param void
* @return void
*/
void Game::run() {
    // First render to display the initial game board
    window.fullRender(highlightMoves, lastMoves, Pieces, whiteDown, buttons, &wTimer, &bTimer);

    while (gameRunning) {
        // Check if a piece is selected for dragging
        if (PieceSelected)
            DragPiece();

        // Check for the 50-move rule
        if (halfMoveNumber >= 50) {
            state = 2;  // Set state to draw if 50-move rule is reached
            break;
        }

        // Check for online play
        if (communication != nullptr) {
            if (communication->isConnected) {
                isPlayingOnline = true;
            }
        }

        if (isPlayingOnline) {
            // Update player color and handle online communication
            whiteDown = communication->isWhite;
            std::string read = communication->read();

            if (!read.empty()) {
                // Handle online commands
                if (read == "l") {
                    if (isWhite()) {
                        state = 0;  // White lost
                    } else {
                        state = 1;  // Black lost
                    }
                    break;
                } else if (read == "close") {
                    // Close communication in case of disconnect
                    communication->close();
                    delete communication;
                    communication = nullptr;
                    isPlayingOnline = false;
                    io_context.stop();
                    break;
                } else if (read == "d") {
                    state = 2;  // Draw
                    break;
                }

                // Update the game state based on the received FEN string
                if (whiteTurn != isWhite()) {
                    Pieces = FenImport(read);
                    moveHistory.push_back(FenExport(Pieces, whiteTurn, halfMoveNumber));
                }
            }
        }

        wTimer.timeText = std::to_string((float)wTimer.getTime());
        bTimer.timeText = std::to_string((float)bTimer.getTime()); //TODO necessary?


        // Handle user input events
        handleEvents();

        // Update the game display
        window.fullRender(highlightMoves, std::vector<glm::ivec2>(lastMoves.end() - 2, lastMoves.end()), Pieces,
                          whiteDown, buttons, &wTimer, &bTimer);

        // Display promotion options if a pawn is promoting
        if (isPromoting) {
            window.displayPromotionOptions(lastMoves[lastMoves.size() - 1], whiteTurn);
        }

        // Display the updated game window
        window.display();

        // Check for game end conditions
        if (state != -1) {
            break;
        }
    }
    // Display end-game messages based on the final state
    //0=white lost 1=black lost 2=draw else=quit
    if (state == 2) {
        window.displayMessage("Draw");
    } else if (state == 0) {
        window.displayMessage("White lost");
    } else if (state == 1) {
        window.displayMessage("Black lost");
    }
    if (state != 1 && state != 3) {
        state = -1;
        lastMoves = {{1000,1000}};
        highlightMoves = {{1000,1000}};
        FenImport(defaultFen);
        moveHistory = {};
        gameRunning = true;
        run();
    }
}
/**
* Places pieces if it works
* @param void
* @return void
*/
void Game::placePiece() {
    // Get the mouse position in board coordinates
    glm::ivec2 MousePosition = getMousePosition(whiteDown, window.squareSize);
    
    // Store the old position of the selected piece
    glm::vec2 oldPos = highlightMoves[0];
    
    // Get the current number of pieces on the board
    u_long sizeOfPieces = Pieces.size();
    
    // Check if it's the first move of the game
    if (counter == 0) {
        // Check if it's the player's turn and if move is possible
        if (selectedPiece->move(MousePosition, highlightMoves[0], Pieces, whiteTurn, isPlayingOnline, isWhite())) {
            
            // Rotate the board if needed
            if (rotate_board) {
                whiteDown = !whiteDown;
            }

            // Check for pawn promotion and handle it
            if (!handlePromotion(selectedPiece, sizeOfPieces != Pieces.size())) {
                // Start or pause the timer based on move history
                if (moveHistory.size() == 1) 
                    bTimer.startPause(); 
                else {
                    wTimer.startPause();
                    bTimer.startPause();
                }

                // Switch turns and update move history
                whiteTurn = !whiteTurn;
                moveHistory.push_back(FenExport(Pieces,whiteTurn,halfMoveNumber));
            }

            // Check for checkmate and update the game state
            handleCheckmate();

            // Store the selected piece and its movement in lastMoves
            lastPiece = selectedPiece;
            lastMoves.emplace_back(oldPos);
            lastMoves.emplace_back(selectedPiece->getPos());

            // Reset the highlightMoves
            highlightMoves = {{1000, 1000}};

            // Send the move to the opponent in online play
            if (isPlayingOnline && state == -1) {
                std::string temp = FenExport(Pieces,whiteTurn,halfMoveNumber); // WRONG PROMOTION IGNORED
                communication->send(temp);
            }
        }
    } else {
        // Reset the counter and restore the previous board state
        counter = 0;
        Pieces = FenImport(moveHistory[moveHistory.size() - 1]);
    }

    // Reset PieceSelected flag
    PieceSelected = false;
}

/**
* handles the promotion of a pawn
* @param std::shared_ptr<Piece> selectedPiece
* @param bool Captured
* @return bool
*/
bool Game::handlePromotion(const std::shared_ptr<Piece> &selectedPiece, bool Captured) {
    // get piece and check if it's a pawn
    std::shared_ptr <Pawn> derivedPtr = std::dynamic_pointer_cast<Pawn>(selectedPiece);
    if (derivedPtr != nullptr) {
        // check if the pawn is on the last rank and return true if it is
        if (((selectedPiece->getPos().y == 0 && whiteTurn) || (selectedPiece->getPos().y == 7 && !whiteTurn))) {
            isPromoting = true;
            return true;
        }
    }
    return false;
}

/**
* Handles checkmate
* @param void
* @return void
*/
void Game::handleCheckmate() {
    // Check if there are no legal moves left
    bool no_legal_moves = true;
    bool check = false;
    for (const auto& i: Pieces) {
        if (i->white == whiteTurn && !i->findMoves(Pieces)) {
            no_legal_moves = false;
        }
        // Check if the king is in check
        std::shared_ptr <King> kingPointerDerived = std::dynamic_pointer_cast<King>(i);
        if (kingPointerDerived != nullptr && i->white == whiteTurn) {
            if (i->isKingInCheck(Pieces)) {
                check = true;
            }
        }
    }
    // Update the game state based on the checkmate status
    if (no_legal_moves) {
        // send state to opponent and update game state
        if (!check) {
            state = 2;
            communication->send("d");
        } else if (whiteTurn) {
            state = 0;
            if (isPlayingOnline && !isWhite()) {
                communication->send("l");
            }
        } else {
            state = 1;
            if (isPlayingOnline && isWhite()) {
                communication->send("l");
            }
        }
    }
}

/**
* Handles all SDL events
* @param void
* @return void
*
*/
void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                // Check if the left mouse button is pressed and handle the button clicks
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Check if the resign button is pressed and handle the resign
                    if (buttons[0]->hovered()) {
                        if (whiteTurn) {
                            state = 0;
                        } else {
                            state = 1;
                        }
                        break;
                    }
                    // Check if the online button is pressed and handle the online play
                    if (buttons[1]->hovered()) {
                        // If the player is not playing online, start the communication. If not, close the communication
                        if (!isPlayingOnline && communication == nullptr) {
                            textBox ipBox = textBox(window.windowx / 2 - window.windowx/4, window.windowy / 2 - window.windowy/4);
                            std::string ip = window.TextBox(ipBox);

                            // some weird character at the end of the string is causing problems 
                            if (ip == "localhost") {
                                ip = "127.0.0.1";
                            } else if (ip == "close") {
                                gameRunning = false;
                                state = 3;
                                break;
                            } else if (ip == "no") {
                                // if cancel then don't start online mode
                                continue;
                            }
                            Pieces = FenImport(defaultFen);
                            std::thread t([&]() {
                                boost::asio::io_context::work work(io_context);
                                io_context.run();
                            });
                            t.detach();
                            communication = new Communication(io_context, ip);
                            continue;
                            // start a thread which starts the io_context.run()
                        } else {
                            Pieces = FenImport(defaultFen);
                            communication->send("close");
                            communication->close();
                            delete communication;
                            communication = nullptr;
                            isPlayingOnline = false;
                            io_context.stop();
                        }
                    }
                    // Toggle automatic board rotation
                    if (buttons[2]->hovered()) {
                        rotate_board = !rotate_board;
                    }

                    // Check if a piece is promoting and handle the promotion else select a piece
                    if (isPromoting)
                        handlePromotionPieceSelection(getMousePosition(whiteDown, window.squareSize));
                    else
                        selectPiece();

                // Check if the right mouse button is pressed and highlight the selected field
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    glm::ivec2 mousePos = getMousePosition(whiteDown, window.squareSize);
                    auto position = std::find(highlightMoves.begin(), highlightMoves.end(), mousePos);
                    if (position == highlightMoves.end()) {
                        highlightMoves.push_back(mousePos);
                    } else if (std::distance(highlightMoves.begin(), position) != 0) {
                        highlightMoves.erase(position);
                    }
                }
                break;

            // Check if the window is closed and quit the game
            case SDL_QUIT:
                state = 3;
                gameRunning = false;
                break;
            // Check if the mouse button is released and place the piece
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && selectedPiece != nullptr)// && !isPromoting
                    placePiece();
                break;
            // Check buttons to move forwards and backwards in the move history
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                        counter = 0;
                        Pieces = FenImport(moveHistory[moveHistory.size() - 1]);
                        break;
                    case SDLK_UP:
                        counter = moveHistory.size() - 1;
                        Pieces = FenImport(moveHistory[moveHistory.size() - (1 + counter)]);
                        break;
                    case SDLK_q:
                        state = 3;
                        gameRunning = false;
                        break;
                    case SDLK_LEFT:
                        if (counter < moveHistory.size() - 1) {
                            counter++;
                            std::string lastFen = moveHistory[moveHistory.size() - (1 + counter)];
                            Pieces = FenImport(lastFen);
                        } else {
                            counter = moveHistory.size() - 1;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (counter >= 1) {
                            counter--;
                        }
                        std::string lastFen = moveHistory[moveHistory.size() - (1 + counter)];
                        Pieces = FenImport(lastFen);
                        break;
                }
        }
    }
}
/**
* Handles the promotion of a pawn
* @param glm::vec2 selection
* @return void
* 
*/
void Game::handlePromotionPieceSelection(glm::vec2 selection) {
    // Check if the selection is valid and promote the pawn
    if ((int) selection.x == lastMoves[lastMoves.size() - 1].x) {
        // Remove the pawn and add the new piece
        Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
        switch (whiteTurn ? (int) selection.y : 7 - (int) selection.y) {
            case 0:
                Pieces.push_back(std::make_shared<Queen>(lastMoves[lastMoves.size() - 1], whiteTurn));
                break;
            case 1:
                Pieces.push_back(std::make_shared<Rook>(lastMoves[lastMoves.size() - 1], whiteTurn));
                break;
            case 2:
                Pieces.push_back(std::make_shared<Bishop>(lastMoves[lastMoves.size() - 1], whiteTurn));
                break;
            case 3:
                Pieces.push_back(std::make_shared<Knight>(lastMoves[lastMoves.size() - 1], whiteTurn));
                break;
            default:
                break;
        }
        // Reset the promotion flag and switch turns
        isPromoting = false;
        whiteTurn = !whiteTurn;
        handleCheckmate();
        moveHistory.push_back(FenExport(Pieces,whiteTurn,halfMoveNumber));
        // continue the timers
        wTimer.startPause();
        bTimer.startPause();
    }
}

/**
* Drags the selected piece
* @param void
* @return void
*/
void Game::DragPiece() {
    glm::vec2 newPos = getMousePosition(whiteDown, window.squareSize);
    newPos -= 0.5;
    selectedPiece->setPos(newPos);
}

/**
* Selects a piece
* @param void
* @return void
*/
void Game::selectPiece() {
    // Get the mouse position in board coordinates
    glm::ivec2 MousePosition = getMousePosition(whiteDown, window.squareSize);
    selectedPiece = getMatchingPiece(MousePosition, Pieces);
    // check if piece exists on the selected field
    if (selectedPiece != nullptr) {
        selectedPiece->findMoves(Pieces);
        highlightMoves = {selectedPiece->getPos()};
        for (auto i: selectedPiece->legalMoves)
            highlightMoves.emplace_back(i);

        PieceSelected = true;
    }
}

/**
* Returns a Pieces list of a FEN string
* @param std::string FenString
* @return std::vector <std::shared_ptr<Piece>>
*/
std::vector <std::shared_ptr<Piece>> Game::FenImport(const std::string &FenString) {
    std::vector <std::shared_ptr<Piece>> pieces;
    int countx = 0;
    int county = 0;
    std::string delimiter = " ";
    uint32_t pos = FenString.find(delimiter);
    std::string PositionFen = FenString.substr(0, pos);
    std::string metadataFen = FenString.substr(pos + 1);

    // Loop through the FEN string and create the pieces
    for (char c: PositionFen) {
        if (std::isdigit(c)) {
            int i = c - '0';
            countx += i;
        } else if (c == '/') {
            county += 1;
            countx = 0;
        } else if (std::isalpha(c)) {
            switch (tolower(c)) {
                case 'k':
                    pieces.push_back(std::make_shared<King>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'n':
                    pieces.push_back(std::make_shared<Knight>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'p':
                    pieces.push_back(std::make_shared<Pawn>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'r':
                    pieces.push_back(std::make_shared<Rook>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'b':
                    pieces.push_back(std::make_shared<Bishop>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'q':
                    pieces.push_back(std::make_shared<Queen>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
            }
        }
    }
    // Process the captured metadata if needed
    int count = 0;

    if (metadataFen[count] == 'w') {
        whiteTurn = true;
    } else if (metadataFen[count] == 'b') {
        whiteTurn = false;
    } else {
        throw std::invalid_argument("white or black error");
    }
    count++;
    count++;

    // Check if castling is possible and update the pieces
    if (metadataFen[count] == '-') {
        for (const auto& i: pieces) {
            std::shared_ptr <King> Kings = std::dynamic_pointer_cast<King>(i);
            if (Kings != nullptr) {
                Kings->hasMoved = true;
            }
        }
        count++;
    } else {
        if (metadataFen[count] == 'K') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 7}, pieces);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }

        if (metadataFen[count] == 'Q') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 7}, pieces);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }

        if (metadataFen[count] == 'k') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 0}, pieces);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (!derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }
        if (metadataFen[count] == 'q') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 0}, pieces);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (!derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            ++count;
        }
    }
    count++;

    // Check if en passant is possible and update the pieces
    std::string abc = "abcdefgh12345678";
    if (std::find(abc.begin(), abc.end(), metadataFen[count]) != abc.end() &&
        std::find(abc.begin(), abc.end(), metadataFen[count + 1]) != abc.end()) {
        u_long pos_x = abc.find(metadataFen[count]);
        u_long pos_y = 8 - (metadataFen[count + 1] - '0');
        std::shared_ptr <Piece> derivedPtr = getMatchingPiece({pos_x, whiteTurn ? pos_y + 1 : pos_y - 1}, pieces);
        std::shared_ptr <Pawn> enPassant = std::dynamic_pointer_cast<Pawn>(derivedPtr);
        if (enPassant != nullptr) {
            enPassant->isEnPassantVulnerable = true;
        } else {

            throw std::invalid_argument("Invalid Fen");
        }

        count++;
    }
    count++;
    // Half and full move numbers
    if (std::isdigit(metadataFen[count]) && std::isdigit(metadataFen[count + 1])) {
        std::string digits;
        digits += metadataFen[count];
        digits += metadataFen[count + 1];
        halfMoveNumber = std::stoi(digits);
        count++;
    } else if (std::isdigit(metadataFen[count]) && metadataFen[count + 1] == ' ') {
        halfMoveNumber = metadataFen[count] - '0';
    }
    count += 2;

    if (metadataFen[count]) {
        fullMoveNumber = stoi(metadataFen.substr(count));
    }
    return pieces;
}
