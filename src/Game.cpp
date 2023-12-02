//necessary for windows
#include <functional>
#include <future>

#define SDL_MAIN_HANDLED


#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <SDL2/SDL.h>
#include <iostream>
#include <bits/stdc++.h>

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


#define RESIGN 1
#define ONLINE 0
// TODO: Threefold FIVEFOLD seventy move rule insufficient material?

//constructor of class Game (the main class)
Game::Game(std::string fen) : window("ChessQLD") {

    Pieces = FenImport(fen);
    moveHistory.push_back(fen);

    wTimer = Timer();
    bTimer = Timer();
    buttons = {new Button("resign"), new Button("online"), new Button("rotate")};
    window.initButtons(buttons);
    run();
}

Game::~Game() {
    window.cleanUp();
}


void Game::run() {
    window.fullRender(highlightMoves, lastMoves, Pieces, whiteDown, buttons, &wTimer, &bTimer);
    while (gameRunning) {

        if (PieceSelected)
            DragPiece();
        if (halfMoveNumber >= 50) {
            state = 2;
            break;
        }
        if (communication != nullptr) {
            if (communication->isConnected) {
                isPlayingOnline = true;
            }
        }
        if (isPlayingOnline) {
            whiteDown = communication->isWhite;
            std::string read = communication->read();
            if (!read.empty()) {
                if (read == "l") {
                    if (isWhite()) {
                        state = 0;
                    } else {
                        state = 1;
                    }
                    break;
                } else if (read == "close") {
                    communication->close();
                    delete communication;
                    communication = nullptr;
                    isPlayingOnline = false;
                    io_context.stop();
                    break;
                } else if (read == "d") {
                    state = 2;
                    break;
                }
                if (whiteTurn != isWhite()) {
                    Pieces = FenImport(read);
                    moveHistory.push_back(FenExport(Pieces));
                }
            }
        }

        wTimer.timeText = std::to_string(wTimer.getTicks() / 1000.f);
        bTimer.timeText = std::to_string(bTimer.getTicks() / 1000.f); //TODO necessary?


        handleEvents();

        // TODO: add a way to continue running code while waiting for response



        window.fullRender(highlightMoves, std::vector<glm::ivec2>(lastMoves.end() - 2, lastMoves.end()), Pieces,
                          whiteDown, buttons, &wTimer, &bTimer);
        if (isPromoting) {
            window.displayPromotionOptions(lastMoves[lastMoves.size() - 1], whiteTurn);
        }
        window.display();
        if (state != -1) {
            break;
        }
    }
    //0=white lost 1=black lost 2=draw else=quit
    if (state == 2) {
        gameRunning = false;
        window.displayWelcomeMessage("Draw");
        return;
    } else if (state == 0) {
        gameRunning = false;
        window.displayWelcomeMessage("White lost");
        return;
    } else if (state == 1) {
        gameRunning = false;
        window.displayWelcomeMessage("Black lost");
        return;
    }
}

void Game::placePiece() {
    glm::ivec2 MousePosition = getMousePosition(whiteDown, window.squareSize);
    glm::vec2 oldPos = highlightMoves[0];
    int sizeOfPieces = Pieces.size();
    if (counter == 0) {
        //if ((isPlayingOnline && (communication->isWhite == whiteTurn)) || !isPlayingOnline) {
        if (selectedPiece->move(MousePosition, highlightMoves[0], Pieces, whiteTurn, isPlayingOnline, isWhite())) {
            if (rotate_board) {
                whiteDown = !whiteDown;
            }

            if (!handlePromotion(selectedPiece, sizeOfPieces != Pieces.size())) {
                if (moveHistory.size() == 2) {
                    bTimer.startPause();
                } else if (moveHistory.size() == 3) {
                    bTimer.startPause();
                    wTimer.startPause();
                } else {
                    if (wTimer.isPaused) {
                        std::cout << "unpausing white" << std::endl;
                        wTimer.startPause();
                        bTimer.startPause();
                    } else {
                        std::cout << "pausing white" << std::endl;
                        bTimer.startPause();
                        wTimer.startPause();
                    }
                }
                whiteTurn = !whiteTurn;
                //         ++halfMoveNumber;
                moveHistory.push_back(FenExport(Pieces));
            }
            handleCheckmate();
            lastPiece = selectedPiece;
            lastMoves.push_back(oldPos);
            lastMoves.push_back(selectedPiece->getPos());
            highlightMoves = {{1000, 1000}};
            if (isPlayingOnline && state == -1) {
                std::cout << "should send" << std::endl;
                std::string temp = FenExport(Pieces); //WRONG PROMOTION IGNORED
                communication->send(temp);
            }
        }

    } else {
        counter = 0;
        Pieces = FenImport(moveHistory[moveHistory.size() - 1]);
    }

    //highlightMoves.push_back(selectedPiece->getPos());
    PieceSelected = false;
}

bool Game::handlePromotion(std::shared_ptr <Piece> selectedPiece, bool Captured) {
    std::shared_ptr <Pawn> derivedPtr = std::dynamic_pointer_cast<Pawn>(selectedPiece);
    if (derivedPtr != nullptr) {
        if (((selectedPiece->getPos().y == 0 && whiteTurn) || (selectedPiece->getPos().y == 7 && !whiteTurn))) {
            isPromoting = true;
            return true;
        }
    }
    return false;
}

void Game::handleCheckmate() {
    std::cout << "handling checkmate" << std::endl;
    bool no_legal_moves = true;
    bool check = false;
    for (auto i: Pieces) {
        if (i->white == whiteTurn && !i->findMoves(Pieces)) {
            no_legal_moves = false;
        }

        std::shared_ptr <King> kingPointerDerived = std::dynamic_pointer_cast<King>(i);
        if (kingPointerDerived != nullptr && i->white == whiteTurn) {
            if (i->isKingInCheck(Pieces)) {
                check = true;
            }
        }
    }

    if (no_legal_moves) {
        std::cout << "no legal moves" << std::endl;
        if (!check) {
            std::cout << "king is not in check" << std::endl;
            state = 2;
            communication->send("d");
        } else if (whiteTurn) {
            std::cout << "white lost" << std::endl;
            state = 0;
            if (isPlayingOnline && !isWhite()) {
                std::cout << "sending l" << std::endl;
                communication->send("l");
            }
        } else {
            std::cout << "black lost" << std::endl;
            state = 1;
            if (isPlayingOnline && isWhite()) {
                std::cout << "sending l" << std::endl;
                communication->send("l");
            }
        }
    }
}

//prolly hashmaps of all pieces' moves im too stupid for this
void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (buttons[0]->hovered()) {
                        if (whiteTurn) {
                            state = 0;
                        } else {
                            state = 1;
                        }
                        break;
                    }
                    if (buttons[1]->hovered()) {
                        std::cout << "clicked online button" << std::endl;
                        if (!isPlayingOnline && communication == nullptr) {
                            Pieces = FenImport("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                            std::thread t([&]() {
                                boost::asio::io_context::work work(io_context);
                                io_context.run();
                            });
                            t.detach();
                            communication = new Communication(io_context);
                            // start a thread which starts the io_context.run()
                        } else {
                            Pieces = FenImport("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                            communication->send("close");
                            communication->close();
                            delete communication;
                            communication = nullptr;
                            isPlayingOnline = false;
                            io_context.stop();
                        }
                    }
                    if (buttons[2]->hovered()) {
                        rotate_board = !rotate_board;
                    }

                    if (isPromoting)
                        handlePromotionPieceSelection(getMousePosition(whiteDown, window.squareSize));

                    else
                        selectPiece();
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    glm::ivec2 mousePos = getMousePosition(whiteDown, window.squareSize);
                    std::vector<glm::ivec2>::iterator position = std::find(highlightMoves.begin(), highlightMoves.end(),
                                                                           mousePos);
                    if (position == highlightMoves.end()) {
                        highlightMoves.push_back(mousePos);
                    } else if (std::distance(highlightMoves.begin(), position) != 0) {
                        highlightMoves.erase(position);
                    }
                }

                break;

            case SDL_QUIT:
                state = 3;
                gameRunning = false;
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && selectedPiece != nullptr)// && !isPromoting
                    placePiece();
                break;

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

void Game::handlePromotionPieceSelection(glm::vec2 selection) {
    if ((int) selection.x == lastMoves[lastMoves.size() - 1].x) {
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

        isPromoting = false;
        whiteTurn = !whiteTurn;
        handleCheckmate();
        moveHistory.push_back(FenExport(Pieces));
        wTimer.startPause();
        bTimer.startPause();
        bTimer.startPause();
        wTimer.startPause();
    }
}

void Game::DragPiece() {
    glm::vec2 newPos = getMousePosition(whiteDown, window.squareSize);
    newPos -= 0.5;
    selectedPiece->setPos(newPos);
}

void Game::selectPiece() {
    glm::ivec2 MousePosition = getMousePosition(whiteDown, window.squareSize);
    selectedPiece = getMatchingPiece(MousePosition, Pieces);

    if (selectedPiece != nullptr) {
        selectedPiece->findMoves(Pieces);
        highlightMoves = {selectedPiece->getPos()};
        for (auto i: selectedPiece->legalMoves)
            highlightMoves.push_back(i);

        PieceSelected = true;
    }
}

std::vector <std::shared_ptr<Piece>> Game::FenImport(std::string FenString) {
    std::vector <std::shared_ptr<Piece>> piecesVector;
    int countx = 0;
    int county = 0;
    std::string delimeter = " ";
    int pos = FenString.find(delimeter);
    std::string PositionFen = FenString.substr(0, pos);
    std::string metadataFen = FenString.substr(pos + 1);


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
        throw std::invalid_argument("white or black erro");
    }
    count++;
    count++;

    if (metadataFen[count] == '-') {
        for (auto i: piecesVector) {
            std::shared_ptr <King> Kings = std::dynamic_pointer_cast<King>(i);
            if (Kings != nullptr) {
                Kings->hasMoved = true;
            }
        }
        count++;
    } else {
        if (metadataFen[count] == 'K') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 7}, piecesVector);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }

        if (metadataFen[count] == 'Q') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 7}, piecesVector);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }

        if (metadataFen[count] == 'k') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{7, 0}, piecesVector);
            std::shared_ptr <Rook> derivedPtr = std::dynamic_pointer_cast<Rook>(pieceTemp);
            if (derivedPtr != nullptr) {
                if (!derivedPtr->white) {
                    derivedPtr->hasMoved = false;
                }
            }
            count++;
        }
        if (metadataFen[count] == 'q') {
            std::shared_ptr <Piece> pieceTemp = getMatchingPiece(glm::vec2{0, 0}, piecesVector);
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

    std::string abc = "abcdefgh12345678";
    if (std::find(abc.begin(), abc.end(), metadataFen[count]) != abc.end() &&
        std::find(abc.begin(), abc.end(), metadataFen[count + 1]) != abc.end()) {
        int posx = abc.find(metadataFen[count]);
        int posy = 8 - (metadataFen[count + 1] - '0');
        std::shared_ptr <Piece> derivedPtr = getMatchingPiece({posx, whiteTurn ? posy + 1 : posy - 1}, piecesVector);
        std::shared_ptr <Pawn> enPassant = std::dynamic_pointer_cast<Pawn>(derivedPtr);
        if (enPassant != nullptr) {
            enPassant->isEnPassantVulnerable = true;
        } else {

            throw std::invalid_argument("Invalid Fen");
        }

        count++;
    }
    count++;
    if (std::isdigit(metadataFen[count]) && std::isdigit(metadataFen[count + 1])) {
        std::string digits = "";
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
    return piecesVector;
}


std::string Game::FenExport(std::vector <std::shared_ptr<Piece>> piecesVector) {
    std::map <std::string, std::shared_ptr<Piece>> posMap;
    std::string FenExportString = "";
    std::string enPassantSquare = "-";
    for (auto i: piecesVector) {
        posMap[glm::to_string(i->getPos())] = i;
    }
    int count = 0;
    int whiteSpaces = 0;
    while (count < 64) {
        int y = (int) count / 8;
        int x = count % 8;


        if (x == 0 && count != 0) {
            if (whiteSpaces != 0) {
                FenExportString += std::to_string(whiteSpaces);
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
            std::shared_ptr <Pawn> pawnPointerDerived = std::dynamic_pointer_cast<Pawn>(i->second);
            if (pawnPointerDerived != nullptr) {
                FenExportString += i->second->white ? "P" : "p";
                std::string abc = "abcdefgh";
                if (pawnPointerDerived->isEnPassantVulnerable) {
                    enPassantSquare = abc[x];
                    enPassantSquare += i->second->white ? std::to_string(8 - (y + 1)) : std::to_string(8 - (y - 1));
                }
            } else if (std::shared_ptr < Rook > rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second)) {
                FenExportString += i->second->white ? "R" : "r";
            } else if (std::shared_ptr < King > kingPointerDerived = std::dynamic_pointer_cast<King>(i->second)) {
                FenExportString += i->second->white ? "K" : "k";
            } else if (std::shared_ptr < Bishop > bishopPointerDerived = std::dynamic_pointer_cast<Bishop>(i->second)) {
                FenExportString += i->second->white ? "B" : "b";
            } else if (std::shared_ptr < Knight > knightPointerDerived = std::dynamic_pointer_cast<Knight>(i->second)) {
                FenExportString += i->second->white ? "N" : "n";
            } else if (std::shared_ptr < Queen > queenPointerDerived = std::dynamic_pointer_cast<Queen>(i->second)) {
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
        std::shared_ptr <King> kingPointerDerived = std::dynamic_pointer_cast<King>(king->second);
        if (kingPointerDerived != nullptr) {
            if (kingPointerDerived->white && !kingPointerDerived->hasMoved) {
                auto i = posMap.find(glm::to_string(glm::vec2{7, 7}));
                if (i != posMap.end()) {
                    std::shared_ptr <Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
                    if (rookPointerDerived != nullptr) {
                        if (rookPointerDerived->white && !rookPointerDerived->hasMoved) {
                            FenExportString += 'K';
                        }
                    }
                }
                i = posMap.find(glm::to_string(glm::vec2{0, 7}));
                if (i != posMap.end()) {
                    std::shared_ptr <Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
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
        std::shared_ptr <King> kingPointerDerived = std::dynamic_pointer_cast<King>(king->second);
        if (kingPointerDerived != nullptr) {
            if (!kingPointerDerived->white && !kingPointerDerived->hasMoved) {
                auto i = posMap.find(glm::to_string(glm::vec2{0, 0}));
                if (i != posMap.end()) {
                    std::shared_ptr <Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
                    if (rookPointerDerived != nullptr) {
                        if (!rookPointerDerived->white && !rookPointerDerived->hasMoved) {
                            FenExportString += 'k';
                        }
                    }
                }
                i = posMap.find(glm::to_string(glm::vec2{7, 0}));
                if (i != posMap.end()) {
                    std::shared_ptr <Rook> rookPointerDerived = std::dynamic_pointer_cast<Rook>(i->second);
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