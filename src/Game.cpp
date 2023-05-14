
#include <cmath>
#include <glm/fwd.hpp>

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

Game::Game() : window("Schach"){
    run();
}
void Game::run() {
    while (gameRunning)
    {
        window.updateWindowSize();
        if (isPieceSelected && selectedEntity != nullptr)
        {
           DragPiece();
        }
        handleEvents();
        window.fullRender(lastPositions, Pieces);
    }
}

Game::~Game() {
    window.cleanUp();
}

void Game::DragPiece() {
        
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    float newPos_x = (float)Mouse_x / (float)window.squareSize - 0.5 ;
    float newPos_y = (float)Mouse_y / (float)window.squareSize - 0.5;
    selectedEntity->setPos(glm::vec2(newPos_x, newPos_y));
        
}

void Game::selectPiece() {
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    isPieceSelected = true;
    selectedEntity = getMatchingPiece(Mouse_x/window.squareSize, Mouse_y/window.squareSize, Pieces);
    if (selectedEntity == nullptr) {
        lastPositions = {{1000, 1000}};
    }
    else {
    selectedEntity->findMoves(Pieces);    
    lastPositions = {selectedEntity->getPos()};
    }
}

void Game::placePiece() {
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    selectedEntity->move(glm::vec2{Mouse_x/window.squareSize,Mouse_y/window.squareSize}, lastPositions[0]);
    lastPositions.push_back(selectedEntity->getPos());
    isPieceSelected = false;
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
