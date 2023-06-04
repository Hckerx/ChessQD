#include "piece.hpp"
#include "util.hpp"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <ctime>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include <algorithm>
#include <king.hpp>

Piece::Piece(glm::vec2 p_pos, bool white)
:pos(p_pos), white(white)
{
}

SDL_Rect Piece::getCurrentFrame()
{
	return currentFrame;
}

bool Piece::findIndMoves(std::vector<std::shared_ptr<Piece>>& Pieces, int x, int y){
    std::shared_ptr<Piece> hypotheticalPiece = getMatchingPiece(glm::vec2{x, y}, Pieces);
    if (x > 7 || x < 0 || y > 7 || y < 0) {
        return false;
    }
    if (hypotheticalPiece == nullptr)
    {
        legalMoves.push_back(glm::vec2(x, y));
        return true;
    }
    else if (hypotheticalPiece->white == white)
    {
        return false;
    }
    else
    {
        legalMoves.push_back(glm::vec2(x, y));
        return false;
    }
}

bool Piece::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn) {
    if (whiteTurn == white) {
        for (glm::vec2 i: legalMoves) {
            if (i == newPos)	{
                std::shared_ptr<Piece> hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y}, Pieces);
                if (hypoPiece != nullptr) {
                    std::vector<std::shared_ptr<Piece>>::iterator position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()){ // == myVector.end() means the element was not found
                        Pieces.erase(position);
                    }
                }
                setPos(newPos);
                // for (auto& i : Pieces) {
                //     i->legalMoves.clear();
                // }
                // for (auto& i : Pieces) {
                //     i->findMovesWithoutCheck(Pieces);
                // }
                // for (auto& i : Pieces) {
                //     i->findMovesWithoutCheck(Pieces);
                // }
                return true;

            }
        }
    }
    setPos(oldPos);
    return false;
}



bool Piece::findMoves(std::vector<std::shared_ptr<Piece>>& Pieces) {
    findMovesWithoutCheck(Pieces);
    std::vector<glm::vec2> legalMovescopy = legalMoves;
    std::vector<glm::vec2> newLegalMoves;

    glm::vec2 oldpos = pos;
    
    glm::vec2 PfuschKoordinaten;
    for (auto move : legalMovescopy)
    {
        
        std::shared_ptr<Piece> hypoPiece = getMatchingPiece(glm::vec2{move.x, move.y}, Pieces);
        if (hypoPiece != nullptr) {
            PfuschKoordinaten = hypoPiece->getPos(); // Dies sollte man niemals machen. Wir machen es trotzdem LMFAO
            hypoPiece->setPos({2000,2000});
            pos = move; //neue position setzten um damit die moves alleer pieces zu überprüfen
            if (!isKingInCheck(Pieces)) {
                newLegalMoves.push_back(move);
            }
            hypoPiece->setPos(PfuschKoordinaten);

        } else {
            pos = move;
            if (!isKingInCheck(Pieces)) {
                newLegalMoves.push_back(move);
            }
        }
    }
    pos = oldpos;
    legalMoves = newLegalMoves;
    return legalMoves.empty();
}



bool Piece::isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
    glm::vec2 kingPos;
    for (auto& i : Pieces) {
        i->findMovesWithoutCheck(Pieces);
        std::shared_ptr<King> derivedPtr = std::dynamic_pointer_cast<King>(i); //was macht das?
        if (derivedPtr != nullptr && i->white == white) {
            kingPos = i->pos; 
        }
    }
    // for piece in pieces
    //     piece.findmoves

    for (auto& i : Pieces) {
        if (i->white != white) {
            for (glm::vec2 j : i->legalMoves) {
                if (j == kingPos) {
                    return true;
                }
            }    
        }
    }
    return false;
}
