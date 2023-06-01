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
#include "entity.hpp"

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
        window.fullRender(lastPositions, Pieces, rotate_board);
    }
}

Game::~Game() {
    window.cleanUp();
}

void Game::DragPiece() {

    glm::vec2 newPos = getMousePosition(rotate_board,window.squareSize);

    
    newPos -= 0.5;
    selectedEntity->setPos(newPos);
        
}

void Game::selectPiece() {
    
    glm::ivec2 MousePosition = getMousePosition(rotate_board,window.squareSize);
     
    selectedEntity = getMatchingPiece(MousePosition, Pieces);
    if (selectedEntity == nullptr) {
        lastPositions = {{1000, 1000}};
    }
    else {
        selectedEntity->findMovesWithCheck(Pieces);    
        //selectedEntity->findMoves(Pieces);    
        lastPositions = {selectedEntity->getPos()};
        isPieceSelected = true;
    }
}

void Game::placePiece() {

     glm::ivec2 MousePosition = getMousePosition(rotate_board,window.squareSize);

    if (selectedEntity->move(MousePosition, lastPositions[0], Pieces, white_turn)) {
        std::cout << "------------------------------------------------------------" << std::endl;
        white_turn = !white_turn;
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
    lastPositions.push_back(selectedEntity->getPos());
    isPieceSelected = false;
    std::cout << "------------------------------------------------------------" << std::endl;
}

void Game::handleEvents() {
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
                if (event.button.button == SDL_BUTTON_LEFT && selectedEntity != nullptr) {
                    placePiece();
                }
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q: gameRunning = false; break;
                }
        }
    }
}


