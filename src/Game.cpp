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
    selectedEntity->setPos(newPos);
        
}

void Game::selectPiece() {
    game_started = true;
    
    glm::ivec2 MousePosition = getMousePosition(rotate_board,window.squareSize);
     
    selectedEntity = getMatchingPiece(MousePosition, Pieces);
    if (selectedEntity != nullptr) {
        selectedEntity->findMovesWithCheck(Pieces);    
        //selectedEntity->findMoves(Pieces);    
        //lastPositions.push_back(selectedEntity->getPos());
        lastPositions = {selectedEntity->getPos()};
        isPieceSelected = true;
        hasClickedPiece = true;
    }
    else {
        hasClickedPiece = false;
    }
}

void Game::placePiece() {

     glm::ivec2 MousePosition = getMousePosition(rotate_board,window.squareSize);

    if (selectedEntity->move(MousePosition, lastPositions[0], Pieces, white_turn)) {
        std::cout << "------------------------------------------------------------" << std::endl;
        white_turn = !white_turn;
        handleCheckmate();
        //lastEntities.push_back(selectedEntity);
    } 
    lastPositions.push_back(selectedEntity->getPos());
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
    if (counter <= 0)
        counter = 1;
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
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q: gameRunning = false; break;
                    // case SDLK_LEFT: if (game_started && lastEntities.size() >= 1*counter && lastPositions.size() >= 2*counter){lastEntities[lastEntities.size() - 1*counter]->setPos(lastPositions[lastPositions.size()-2*counter]); white_turn = !white_turn; counter++;} break;
                   // case SDLK_RIGHT: if (game_started && lastEntities.size() >= 1*counter-1 && lastPositions.size() >= 1*counter-1 && counter > 1){counter--; lastEntities[lastEntities.size() - counter]->setPos(lastPositions[lastPositions.size()-counter]); white_turn = !white_turn; } break;
                }
        }
    }
}


