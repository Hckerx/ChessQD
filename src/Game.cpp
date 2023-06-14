#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
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
        //std::vector<glm::vec2> temp = {{1000,1000}};
        window.fullRender(lastPositions, Pieces, whiteDown);
        if (isPromoting) {
            window.displayPromotionOptions(lastPositions[lastPositions.size() - 1], whiteTurn);
        }
        window.display();
    }
}

Game::~Game() {
    window.cleanUp();
}

void Game::DragPiece() {

    glm::vec2 newPos = getMousePosition(whiteDown,window.squareSize);


    newPos -= 0.5;
    selectedPiece->setPos(newPos);

}

void Game::selectPiece() {

    glm::ivec2 MousePosition = getMousePosition(whiteDown,window.squareSize);


    selectedPiece = getMatchingPiece(MousePosition, Pieces);
    if (selectedPiece != nullptr) {
        selectedPiece->findMoves(Pieces);     
        lastPositions = {selectedPiece->getPos()};
        for (auto i: selectedPiece->legalMoves) {
            lastPositions.push_back(i);
        }
        isPieceSelected = true;

    } 
}

void Game::placePiece() {

    glm::ivec2 MousePosition = getMousePosition(whiteDown,window.squareSize);

    /* if (!lastPieces.empty()) {
        if (lastPieces[lastPieces.size()-1]->white) {
            whiteTurn = false;
            } else {
            whiteTurn = true;
        }
    } */
    glm::vec2 oldPos = lastPositions[0];
    if (selectedPiece->move(MousePosition, lastPositions[0], Pieces, whiteTurn)) {
        if (rotate_board) {
            whiteDown=!whiteDown;
        }
        if (!handleProtomotion(selectedPiece)) {
            whiteTurn = !whiteTurn;
        }
        handleCheckmate();
        lastPieces.push_back(selectedPiece);
        lastPositions = {oldPos, selectedPiece->getPos()};
    } 
    //lastPositions.push_back(selectedPiece->getPos());
    isPieceSelected = false;
}

bool Game::handleProtomotion(std::shared_ptr<Piece> selectedPiece)
{
    std::shared_ptr<Pawn> derivedPtr = std::dynamic_pointer_cast<Pawn>(selectedPiece);
    if (derivedPtr != nullptr && ((selectedPiece->getPos().y == 0 && whiteTurn) || (selectedPiece->getPos().y == 7 && !whiteTurn)))
    {
        isPromoting = true;
        return true;
    }
    return false;
}

void Game::handleCheckmate() {
    bool checkmate_white = true;
    bool checkmate_black = true;
    for (auto i : Pieces) {
        if (i->white && !i->findMoves(Pieces)) {
            checkmate_white = false;
        }
        if (!i->white && !i->findMoves(Pieces)) {
            checkmate_black = false;
        }
    }
    if (checkmate_black || checkmate_white) {
        gameRunning = false;
    }
}
//prolly hashmaps of all pieces' moves im too stupid for this
void Game::handleEvents()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    if (!isPromoting)
                        {
                            selectPiece();
                        }
                    else
                    {
                        handlePromotionPieceSelection(getMousePosition(whiteDown, window.squareSize));

                    }
                }
                break;
            case SDL_QUIT:
                gameRunning = false;
                return;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && selectedPiece != nullptr && !isPromoting)
                {
                    placePiece();
                    break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_q:
                                gameRunning = false;
                                break;
                        }
                }
        }
    }
}

void Game::handlePromotionPieceSelection(glm::vec2 selection){
    std::shared_ptr<Piece> lastPiece = lastPieces[lastPieces.size()-1];
    if ((int)selection.x == lastPositions[lastPositions.size() -1].x) {
        switch (whiteTurn ? (int)selection.y : 7 - (int)selection.y) {
            case 0: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Queen>(lastPositions[lastPositions.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                break;
            case 1: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Rook>(lastPositions[lastPositions.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                break;
            case 2: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Bishop>(lastPositions[lastPositions.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                break;
            case 3: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Knight>(lastPositions[lastPositions.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                break;
            default: 
                break;
        } 
    } 
}

