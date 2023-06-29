#include <algorithm>
#include <iterator>
#include <map>
#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <string>
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
#include <bits/stdc++.h>
#include "game.hpp"

Game::Game(std::string fen, bool server) : window("never gonna give you up"), isServer(server) {
    if (window.displayWelcomeMessage("Welcome to ChessQLD")){
        
        Pieces = FenImport(fen);
        moveHistory.push_back(fen);
        if (isPlayingOnline && isServer) {
            isWhite = std::rand() % 2;
            communication = std::make_unique<Communication>(server);
            communication->send(isWhite ? "black\n" : "white\n");
        } else {
            communication = std::make_unique<Communication>(!server);
            if (communication->receive() == "black\n") {
                isWhite = false;
            } else {
                isWhite = true;
            }
        }
        run();
        window.displayWelcomeMessage(whiteTurn ? "White lost" : "Black lost");
    }
}


bool Game::run() {
    while (gameRunning)
    {
        window.updateWindowSize();
        if (isPieceSelected)
        {
            DragPiece();
        }
        if (halfMoveNumber >= 50) {
            draw = true;
            break;
        }
        handleEvents();
        //std::vector<glm::vec2> temp = {{1000,1000}};
        window.fullRender(highlightMoves, std::vector<glm::ivec2>(lastMoves.end() - 2, lastMoves.end()), Pieces, whiteDown);
        if (isPromoting) {
            window.displayPromotionOptions(lastMoves[lastMoves.size() - 1], whiteTurn);
        }
        window.display();
    }
    return draw;
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
        highlightMoves = {selectedPiece->getPos()};
        for (auto i: selectedPiece->legalMoves) {
            highlightMoves.push_back(i);
        }
        isPieceSelected = true;

    } 
}

void Game::placePiece() {

    glm::ivec2 MousePosition = getMousePosition(whiteDown,window.squareSize);

    glm::vec2 oldPos = highlightMoves[0];
    int sizeOfPieces = Pieces.size();
    if (counter == 0) {
        if (selectedPiece->move(MousePosition, highlightMoves[0], Pieces, whiteTurn)) {
            if (rotate_board) {
                whiteDown=!whiteDown;
            }
            if (!handleProtomotion(selectedPiece, sizeOfPieces != Pieces.size())) {
                whiteTurn = !whiteTurn;
                //         ++halfMoveNumber;
                moveHistory.push_back(FenExport(Pieces));
            }
            handleCheckmate();
            lastPieces.push_back(selectedPiece);
            lastMoves.push_back(oldPos);
            lastMoves.push_back(selectedPiece->getPos());
            highlightMoves = {{1000,1000}};
        } 
    } else {
        counter = 0;
        Pieces = FenImport(moveHistory[moveHistory.size() - 1]);
    }

    //highlightMoves.push_back(selectedPiece->getPos());
    isPieceSelected = false;
}

bool Game::handleProtomotion(std::shared_ptr<Piece> selectedPiece, bool Captured)
{
    std::shared_ptr<Pawn> derivedPtr = std::dynamic_pointer_cast<Pawn>(selectedPiece);
    if (derivedPtr != nullptr )
    {
        if (((selectedPiece->getPos().y == 0 && whiteTurn) || (selectedPiece->getPos().y == 7 && !whiteTurn))) {
            isPromoting = true;
            return true;
        }
    } else if (derivedPtr == nullptr || !Captured) {
       halfMoveNumber += 1; 
    }
    return false;
}
// TODO
void Game::handleCheckmate() {
    bool checkmate_white = true;
    bool checkmate_black = true;
    bool check_white = false;
    bool check_black = false;
    for (auto i : Pieces) {
        if (i->white && !i->findMoves(Pieces)) {
            checkmate_white = false;
        }
        if (!i->white && !i->findMoves(Pieces)) {
            checkmate_black = false;
        }
        std::shared_ptr<King> kingPointerDerived = std::dynamic_pointer_cast<King>(i);
        if (kingPointerDerived != nullptr) {
            if (kingPointerDerived->isKingInCheck(Pieces) && kingPointerDerived->white) {
                check_white = true;
            }
            else if (kingPointerDerived->isKingInCheck(Pieces) && !kingPointerDerived->white) {
                check_black = true;
            }
        }
    }
    if (checkmate_black || checkmate_white) {
        gameRunning = false;
        if ((checkmate_black && !check_black) || (checkmate_white && !check_white)) {
            draw = true;
        }
    }
}
//prolly hashmaps of all pieces' moves im too stupid for this
void Game::handleEvents()
{
    if (counter <= 0) {
        counter = 0;
    }
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
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    glm::ivec2 mousePos = getMousePosition(whiteDown, window.squareSize);
                    std::vector<glm::ivec2>::iterator position = std::find(highlightMoves.begin(), highlightMoves.end(), mousePos);
                    if (position == highlightMoves.end()) {
                        highlightMoves.push_back(mousePos);
                    } else if (std::distance(highlightMoves.begin(), position) != 0){
                        highlightMoves.erase(position);
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
                            case SDLK_DOWN:
                                counter = 0;
                                Pieces = FenImport(moveHistory[moveHistory.size() - 1]);
                                break;
                            case SDLK_UP:
                                counter = moveHistory.size() - 1;
                                Pieces = FenImport(moveHistory[moveHistory.size() - (1 + counter)]);
                                break;
                            case SDLK_q:
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
}

void Game::handlePromotionPieceSelection(glm::vec2 selection){
    std::shared_ptr<Piece> lastPiece = lastPieces[lastPieces.size()-1];
    if ((int)selection.x == lastMoves[lastMoves.size() -1].x) {
        switch (whiteTurn ? (int)selection.y : 7 - (int)selection.y) {
            case 0: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Queen>(lastMoves[lastMoves.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                moveHistory.push_back(FenExport(Pieces));
                break;
            case 1: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Rook>(lastMoves[lastMoves.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                moveHistory.push_back(FenExport(Pieces));
                break;
            case 2: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Bishop>(lastMoves[lastMoves.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                handleCheckmate();
                moveHistory.push_back(FenExport(Pieces));
                break;
            case 3: 
                Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), lastPiece), Pieces.end());
                Pieces.push_back(std::make_shared<Knight>(lastMoves[lastMoves.size()-1], whiteTurn));
                isPromoting = false;
                whiteTurn = !whiteTurn;
                //++halfMoveNumber;
                handleCheckmate();
                moveHistory.push_back(FenExport(Pieces));
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
    int count = 0;

    if (metadataFen[count] == 'w') {
        whiteTurn = true;
    } else if (metadataFen[count] == 'b') {
        whiteTurn = false;
    } else {

        throw std::invalid_argument( "white or black erro" );
    }
    count++;
    count++;

    if (metadataFen[count] == '-') {
        for (auto i : piecesVector) {
            std::shared_ptr<King> Kings = std::dynamic_pointer_cast<King>(i);
            if (Kings != nullptr) {
                Kings->hasMoved = true;
            }
        }
        count++;
    }
    else {
        if (metadataFen[count] == 'K') {
            std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 7}, piecesVector);
            std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }

        if (metadataFen[count] == 'Q') {
            std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 7}, piecesVector);
            std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }

        if (metadataFen[count] == 'k') {
            std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 0}, piecesVector);
            std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (!derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }
        if (metadataFen[count] == 'q') {
            std::shared_ptr<Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 0}, piecesVector);
            std::shared_ptr<Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (!derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            ++count;
        } 
    }
    count++;

    std::string abc = "abcdefgh12345678";
    if (std::find(abc.begin(), abc.end(), metadataFen[count]) != abc.end() && std::find(abc.begin(), abc.end(), metadataFen[count+1]) != abc.end()) {
        int posx = abc.find(metadataFen[count]);
        int posy = 8 - (metadataFen[count+1] - '0');
        std::shared_ptr<Piece> derivedPtr = getMatchingPiece({posx, whiteTurn ? posy+1 : posy-1}, piecesVector);
        std::shared_ptr<Pawn> enPassant = std::dynamic_pointer_cast<Pawn>(derivedPtr);
        if (enPassant != nullptr) {
            enPassant->isEnPassantVulnerable = true;
        } else {

            throw std::invalid_argument( "Invalid Fen" );
        }

        count++;
    }
    count++;
    if (std::isdigit(metadataFen[count]) && std::isdigit(metadataFen[count+1])) {
        std::string digits = "";
        digits += metadataFen[count];
        digits += metadataFen[count+1];
        halfMoveNumber = std::stoi(digits);
        count++;
    }
    else if (std::isdigit(metadataFen[count])&& metadataFen[count+1] == ' '){
        halfMoveNumber = metadataFen[count] - '0';
    }
   count+=2;
   
    if (metadataFen[count]) {
        fullMoveNumber = stoi(metadataFen.substr(count));
    }
    return piecesVector;
}



std::string Game::FenExport(std::vector<std::shared_ptr<Piece>> piecesVector) {
    std::map<std::string, std::shared_ptr<Piece>> posMap;
    std::string FenExportString = "";
    std::string enPassantSquare = "-";
    for (auto i : piecesVector) {
        posMap[glm::to_string(i->getPos())] = i; 
    }
    int count = 0;
    int whiteSpaces = 0;
    while (count < 64) {
        int y = (int)count/8;
        int x = count%8;

        
        if (x == 0 && count != 0) {
            if (whiteSpaces != 0) {
                FenExportString += std::to_string(whiteSpaces) ; 
            }
            FenExportString += '/';
            whiteSpaces = 0;

        }         
        auto i = posMap.find(glm::to_string(glm::vec2{x, y}));
        if (i != posMap.end()) {
            if (whiteSpaces != 0) {
                FenExportString += std::to_string(whiteSpaces);
                whiteSpaces = 0;
            }
            std::shared_ptr<Pawn> pawnPointerDerived = std::dynamic_pointer_cast<Pawn>(i->second);
            if (pawnPointerDerived != nullptr) {
                FenExportString += i->second->white ? "P" : "p";
                std::string abc = "abcdefgh";
                if (pawnPointerDerived->isEnPassantVulnerable) {
                    enPassantSquare = abc[x];
                    enPassantSquare += i->second->white ? std::to_string(8-(y+1)) : std::to_string(8-(y-1));
                    std::cout << enPassantSquare << std::endl;
                }
            }
            else if (std::shared_ptr<Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second)) {
                FenExportString += i->second->white ? "R" : "r";
            }
            else if (std::shared_ptr<King> kingPointerDerived = std::dynamic_pointer_cast<King>(i->second)) {
                FenExportString += i->second->white ? "K" : "k";
            }
            else if (std::shared_ptr<Bishop> bishopPointerDerived = std::dynamic_pointer_cast<Bishop>(i->second)) {
                FenExportString += i->second->white ? "B" : "b";
            }
            else if (std::shared_ptr<Knight> knightPointerDerived = std::dynamic_pointer_cast<Knight>(i->second)) {
                FenExportString += i->second->white ? "N" : "n";
            }
            else if (std::shared_ptr<Queen> queenPointerDerived = std::dynamic_pointer_cast<Queen>(i->second)) {
                FenExportString += i->second->white ? "Q" : "q";
            }
        } else {
               whiteSpaces += 1;
        }

        count++;
    }
    FenExportString += ' '; 
    FenExportString += whiteTurn ? 'w' : 'b';
    FenExportString += ' '; 
    auto king = posMap.find(glm::to_string(glm::vec2{4, 7}));
    if (king != posMap.end()) {
        std::shared_ptr<King> kingPointerDerived = std::dynamic_pointer_cast<King>(king->second);
        if (kingPointerDerived != nullptr) {
            if (kingPointerDerived->white && !kingPointerDerived->hasMoved) {
                auto i = posMap.find(glm::to_string(glm::vec2{7, 7}));
                if (i != posMap.end()) {
                    std::shared_ptr<Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
                    if (rookPointerDerived != nullptr) {
                        if (rookPointerDerived->white && !rookPointerDerived->hasMoved) {
                            FenExportString += 'K';
                        }
                    }
                }
                i = posMap.find(glm::to_string(glm::vec2{0, 7}));
                if (i != posMap.end()) {
                    std::shared_ptr<Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
                    if (rookPointerDerived != nullptr) {
                        if (rookPointerDerived->white && !rookPointerDerived->hasMoved) {
                            FenExportString += 'Q';
                        }
                    }
                }
            }
        }
    }

    king = posMap.find(glm::to_string(glm::vec2{4, 0}));
    if (king != posMap.end()) {
        std::shared_ptr<King> kingPointerDerived = std::dynamic_pointer_cast<King>(king->second);
        if (kingPointerDerived != nullptr) {
            if (!kingPointerDerived->white && !kingPointerDerived->hasMoved) {
                auto i = posMap.find(glm::to_string(glm::vec2{0, 0}));
                if (i != posMap.end()) {
                    std::shared_ptr<Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
                    if (rookPointerDerived != nullptr) {
                        if (!rookPointerDerived->white && !rookPointerDerived->hasMoved) {
                            FenExportString += 'k';
                        }
                    }
                }
                i = posMap.find(glm::to_string(glm::vec2{7, 0}));
                if (i != posMap.end()) {
                    std::shared_ptr<Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
                    if (rookPointerDerived != nullptr) {
                        if (!rookPointerDerived->white && !rookPointerDerived->hasMoved) {
                            FenExportString += 'q';
                        }
                    }
                }

            }
        }
    }

    FenExportString += ' ' + enPassantSquare + ' ';
    FenExportString += std::to_string(halfMoveNumber);
    FenExportString += " 0";
    return FenExportString; 
}
