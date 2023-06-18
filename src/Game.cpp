#include <algorithm>
#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <sstream>

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

Game::Game(std::string fen) : window("never gonna give you up"){
    //window.displayWelcomeMessage();
    Pieces = FenImport("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
   //         ++halfMoveNumber;
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
                //++halfMoveNumber;
                handleCheckmate();
                break;
            default: 
                break;
        } 
    } 
}


std::vector<std::shared_ptr<Piece>> Game::FenImport(std::string FenString) {
    std::vector<std::shared_ptr<Piece>> piecesVector;
    int countx = 0;
    int county = 0;
    std::string delimeter = " ";
    int pos = FenString.find(delimeter);
    std::string PositionFen = FenString.substr(0, pos);
    std::string metadataFen = FenString.substr(pos+1);



    for (char c : PositionFen) {
        if (std::isdigit(c)) {
            int i = c - '0';
            countx += i;
        } else if (c == '/') {
            county += 1;
            countx = 0;
        } else if (std::isalpha(c)) {
            switch (tolower(c)) {
                case 'k':
                    piecesVector.push_back(std::make_shared<King>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'n':
                    piecesVector.push_back(std::make_shared<Knight>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'p':
                    piecesVector.push_back(std::make_shared<Pawn>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'r':
                    piecesVector.push_back(std::make_shared<Rook>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'b':
                    piecesVector.push_back(std::make_shared<Bishop>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'q':
                    piecesVector.push_back(std::make_shared<Queen>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
            }
        } 
    }

    // Process the captured metadata if needed
    std::string element = metadataFen;


    int count = 0;

       
            if (element[count] == 'w') {
                whiteTurn = true;
            } else if (element[count] == 'b') {
                whiteTurn = false;
            }
            else {
            throw std::invalid_argument( "Invalid Fen" );
            }
            count++;
            count++;


       
            if (element[count] == '-') {
                for (auto i : piecesVector) {
                    std::shared_ptr<King> Kings = std::dynamic_pointer_cast<King>(i);
                    if (Kings != nullptr) {
                        Kings->hasMoved = true;
                    }
                }
               count++;
            }
            else {
            
           
            
            if (element[count] == 'K') {
                    std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 7}, Pieces);
                    std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
                    if (derivedPtr != nullptr) {
                        if (derivedPtr->white) {
                            derivedPtr->hasMoved = false;
                        }
                }
                count++;
            }
            if (element[count] == 'Q') {
                    std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 7}, Pieces);
                    std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
                    if (derivedPtr != nullptr) {
                        if (derivedPtr->white) {
                            derivedPtr->hasMoved = false;
                        }
                }
                count++;
            }

            if (element[count] == 'k') {
                    std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 0}, Pieces);
                    std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
                    if (derivedPtr != nullptr) {
                        if (derivedPtr->white) {
                            derivedPtr->hasMoved = false;
                        }
                }
                count++;
            }
            if (element[count] == 'q') {
                    std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 0}, Pieces);
                    std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
                    if (derivedPtr != nullptr) {
                        if (derivedPtr->white) {
                            derivedPtr->hasMoved = false;
                        }
                }
            ++count;
            } 
            }
            count++;
        

        std::string abc = "abcdefgh12345678";
        
         if (std::find(abc.begin(), abc.end(), element[count]) != abc.end() && std::find(abc.begin(), abc.end(), element[count+1]) != abc.end()) {
                int posy = 8 - (element[count] - '0');
                std::shared_ptr<Piece> derivedPtr = getMatchingPiece({element[count+1], whiteTurn ? posy+1 : posy - 1}, piecesVector);
                std::shared_ptr<Pawn> enPassant = std::dynamic_pointer_cast<Pawn>(derivedPtr);
                if (enPassant != nullptr) {
                    enPassant->isEnPassantVulnerable = true;
                } else {
                    throw std::invalid_argument( "Invalid Fen" );
                }
            
            count++;
        }
        count++;
        if (element[count]) {
            //fullMoveNumber = (char)element[0] - '0';
            ++count;
        }
        count++;
         if (element[count]) {
           fullMoveNumber = stoi(element);
        }

    

    return piecesVector;
}
