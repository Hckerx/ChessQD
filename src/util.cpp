#include "util.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "piece.hpp"
#include <glm/gtx/string_cast.hpp>
#include <map>

std::shared_ptr <Piece> getMatchingPiece(glm::vec2 field, std::vector <std::shared_ptr<Piece>> &Pieces) {

    for (auto &i: Pieces) {
        if (field == i->pos) {
            return i;
        }
    }
    return nullptr;
}

glm::vec2 getMousePosition(bool whiteDown, int squareSize) {
    int Mouse_x, Mouse_y;
    float Mousex, Mousey;
    SDL_GetMouseState(&Mouse_x, &Mouse_y);


    if (whiteDown) {
        Mousex = (float) Mouse_x / (float) squareSize;
        Mousey = (float) Mouse_y / (float) squareSize;
        Mouse_x = Mouse_x / squareSize;
        Mouse_y = Mouse_y / squareSize;

    } else {
        Mousex = 8 - (float) Mouse_x / (float) squareSize;
        Mousey = 8 - (float) Mouse_y / (float) squareSize;
        Mouse_x = 8 - Mouse_x / squareSize;
        Mouse_y = 8 - Mouse_y / squareSize;
    }
    return glm::vec2{Mousex, Mousey};
}

std::string FenExport(const std::vector <std::shared_ptr<Piece>>& piecesVector,bool whiteTurn,int halfMoveNumber) {
    std::map <std::string, std::shared_ptr<Piece>> posMap;
    std::string FenExportString;
    std::string enPassantSquare = "-";
    for (const auto& i: piecesVector) {
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
