#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "renderWindow.hpp"
#include "util.hpp"

#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "piece.hpp"

#include "game.hpp"

Game::Game() : window("never gonna give you up"){
    run();
}
void Game::run() {
    while (gameRunning)
    {
        window.updateWindowSize();
        if (isPieceSelected)
        {
           DragPiece();
        }
        handleEvents();
        std::vector<glm::vec2> temp = {{1000,1000}};
        window.fullRender((!hasClickedPiece ? temp : std::vector<glm::vec2>(lastPositions.end() - 2, lastPositions.end())), Pieces, rotate_board);
    }
}

Game::~Game() {
    window.cleanUp();
}

void Game::DragPiece() {

    glm::vec2 newPos = getMousePosition(rotate_board,window.squareSize);

    
    newPos -= 0.5;
    selectedPiece->setPos(newPos);
        
}

void Game::selectPiece() {
    
    glm::ivec2 MousePosition = getMousePosition(rotate_board,window.squareSize);
     
    selectedPiece = getMatchingPiece(MousePosition, Pieces);
    if (selectedPiece != nullptr) {
        selectedPiece->findMovesWithCheck(Pieces);    
        //selectedPiece->findMoves(Pieces);    

        if (!lastEntities.empty()) {
            if (lastEntities[lastEntities.size()-1]->white != selectedEntity->white) {
                lastPositions.push_back(selectedEntity->getPos());
            } 
        } else {
                lastPositions.push_back(selectedPiece->getPos());
        }
        //lastPositions = {selectedPiece->getPos()};
        isPieceSelected = true;
        hasClickedPiece = true;
    }
    else {
        hasClickedPiece = false;
    }
}

void Game::placePiece() {

    glm::ivec2 MousePosition = getMousePosition(rotate_board,window.squareSize);

    if (!isOverridable) {
        while (counter > 0) {
            std::cout << "entered loop for what ever reason" << std::endl;
            lastPieces[(lastPieces.size() - 1) - (counter-1)]->setPos(lastPositions[(lastPositions.size() - 1) - (2*(counter-1)+1) ]);
            counter--;
        }
        counter = 0;
    }
    if (!lastPieces.empty()) {
        if (lastPieces[lastPieces.size()-1]->white) {
            white_turn = false;
        } else {
            white_turn = true;
        }
    }
    if (selectedPiece->move(MousePosition, lastPositions[lastPositions.size()-1], Pieces, white_turn)) {
        std::cout << "------------------------------------------------------------" << std::endl;
        white_turn = !white_turn;
        handleCheckmate();
        lastPieces.push_back(selectedPiece);
        lastPositions.push_back(selectedEntity->getPos());
    } 
    lastPositions.push_back(selectedPiece->getPos());
    isPieceSelected = false;
    std::cout << "------------------------------------------------------------" << std::endl;
}
void Game::handleCheckmate() {
        bool checkmate_white = true;
        bool checkmate_black = true;
        for (auto i : Pieces) {
            if (i->white && !i->findMovesWithCheck(Pieces)) {
                checkmate_white = false;
            }
            if (!i->white && !i->findMovesWithCheck(Pieces)) {
                checkmate_black = false;
            }
        }
        if (checkmate_black || checkmate_white) {
            std::cout << "checkmate either of them" << std::endl;
            gameRunning = false;
        }
}
//prolly hashmaps of all pieces' moves im too stupid for this
void Game::handleEvents() {
    if (counter < 0) {
        counter = 0;
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
                    selectPiece();
                    break;
                }
                break;
            case SDL_QUIT:
                gameRunning = false;
                return;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && selectedPiece != nullptr) {
                    placePiece();
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q: gameRunning = false; break;
                    case SDLK_LEFT: if ( (lastPieces.size()-1) >= counter && (lastPositions.size()-1) >= 2*counter){
                        lastPieces[(lastPieces.size() - 1)  - counter]->setPos(lastPositions[(lastPositions.size() - 1)-(2*counter)-1]); 
                        white_turn = !white_turn;
                        counter++;
                    }
                        break;
                    case SDLK_RIGHT: if ( (lastPieces.size()-1) >= 1*counter-1 && lastPositions.size() >= 1*counter-1){
                        lastPieces[(lastPieces.size() - 1) - (counter-1)]->setPos(lastPositions[(lastPositions.size() - 1) - (2*(counter-1)) ]);
                        white_turn = !white_turn; 
                        counter--;
                    } 
                        break;
                }
        }
    }
}


