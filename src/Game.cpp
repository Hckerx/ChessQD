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
        if (isPieceSelected)
        {
           DragPiece();
        }
        handleEvents();
        window.fullRender(lastPositions, Pieces, white_turn);
    }
}

Game::~Game() {
    window.cleanUp();
}

void Game::DragPiece() {
        
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    float Mousex;
    float Mousey;
    if(white_turn){
        Mousex =  (float)Mouse_x/(float)window.squareSize;
        Mousey =  (float)Mouse_y/(float)window.squareSize ; 	
    }
    else
    {
        Mousex =  8-(float)Mouse_x/(float)window.squareSize;;
        Mousey =  8-(float)Mouse_y/(float)window.squareSize; ;
    }

    float newPos_x = Mousex - 0.5 ;
    float newPos_y = Mousey - 0.5;
    selectedEntity->setPos(glm::vec2(newPos_x, newPos_y));
        
}

void Game::selectPiece() {
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    if(white_turn){
        Mouse_x =  Mouse_x/window.squareSize;
        Mouse_y =  Mouse_y/window.squareSize ;	
    }
    else
    {
        Mouse_x =  7-Mouse_x/window.squareSize;
        Mouse_y =  7-Mouse_y/window.squareSize ;
    }
    
     
    selectedEntity = getMatchingPiece(Mouse_x, Mouse_y, Pieces);
    if (selectedEntity == nullptr) {
        lastPositions = {{1000, 1000}};
    }
    else {
        selectedEntity->findMoves(Pieces);    
        lastPositions = {selectedEntity->getPos()};
        isPieceSelected = true;
    }
}

void Game::placePiece() {
    SDL_GetMouseState(&Mouse_x, &Mouse_y);
    if(white_turn){
        Mouse_x =  Mouse_x/window.squareSize;
        Mouse_y =  Mouse_y/window.squareSize ;	
    }
    else
    {
        Mouse_x =  7 - Mouse_x/window.squareSize; 
        Mouse_y =  7 - Mouse_y/window.squareSize;
    }

    if (selectedEntity->move(glm::vec2{Mouse_x,Mouse_y}, lastPositions[0], Pieces, white_turn)) {
        white_turn = !white_turn;
    } 
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
