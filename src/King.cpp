#include "king.hpp"
#include "glm/gtx/string_cast.hpp"
#include "piece.hpp"
#include "glm/glm.hpp"
#include "util.hpp"
#include "rook.hpp"

#include <iostream>
#include <memory>
King::King(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){
    if (white) {
        currentFrame.y = 128;
    }
    else {
        currentFrame.y = 0;
    }
    currentFrame.x = 128*5;
    currentFrame.w = 128;
    currentFrame.h = 128;
}

void King::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>> &Pieces)
{
    // FIXME add special Rules rochade
    legalMoves.clear();
    findIndMoves(Pieces, pos[0] + 1, pos[1] + 1);
    findIndMoves(Pieces, pos[0] + 1, pos[1] - 1);
    findIndMoves(Pieces, pos[0] - 1, pos[1] + 1);
    findIndMoves(Pieces, pos[0] - 1, pos[1] - 1);
    findIndMoves(Pieces, pos[0], pos[1] - 1);
    findIndMoves(Pieces, pos[0], pos[1] + 1);
    if (hasMoved || isCastling) {
        findIndMoves(Pieces, pos[0] + 1, pos[1]);
        findIndMoves(Pieces, pos[0] - 1, pos[1]);
        if (isCastling) { 
            isCastling = false;
        }

        return;
    }
    std::shared_ptr<Piece> pieceTemp;
    // maybe without castling 
    std::shared_ptr<Rook> rookPiece;
        pos.x = pos[0] - 1;
        if (!isKingInCheck(Pieces)) {
            pos.x = pos.x + 1;
            if (findIndMoves(Pieces, pos[0] - 1, pos[1]))
            {
                if (!hasMoved)
                {
                    pieceTemp = getMatchingPiece(glm::vec2{pos[0] - 4, pos[1]}, Pieces);
                    rookPiece = std::dynamic_pointer_cast<Rook>(pieceTemp);
                    if (rookPiece != nullptr)
                    {

                        if (findIndMoves(Pieces, pos[0] - 2, pos[1]))
                        {

                            if (!rookPiece->hasMoved)
                            {
                                rookPiece->findMovesWithoutCheck(Pieces);
                                for (auto i : rookPiece->legalMoves)
                                {
                                    if (i == glm::vec2{pos[0] - 3, pos[1]})
                                    {
                                        legalMoves.push_back(glm::vec2{pos[0] - 2, pos[1]});

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            pos.x = pos[0] + 1;
        }
        pos.x = pos[0] + 1;
        if (!isKingInCheck(Pieces)) {
            pos.x = pos[0] - 1;
            if (findIndMoves(Pieces, pos[0]+1, pos[1]))
            {
                if (!hasMoved)
                {
                    pieceTemp = getMatchingPiece(glm::vec2{pos[0] + 3, pos[1]}, Pieces);
                    rookPiece = std::dynamic_pointer_cast<Rook>(pieceTemp);
                    if (rookPiece != nullptr) {

                        if (!rookPiece->hasMoved) {

                            if (findIndMoves(Pieces, pos[0] + 2, pos[1]))
                            {
                                {
                                    legalMoves.push_back(glm::vec2{pos[0] + 2, pos[1]});
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            pos.x = pos[0] - 1;
        }
}

bool King::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn) {
    if (Piece::move(newPos, oldPos, Pieces, whiteTurn)) {
        if (newPos.x - oldPos.x == 2) {
            std::shared_ptr<Piece> Rook = getMatchingPiece(glm::vec2{7, newPos.y}, Pieces);
            Rook->setPos(glm::vec2{5, newPos.y});
        }
        if (oldPos.x - newPos.x == 2) {
            std::shared_ptr<Piece> Rook = getMatchingPiece(glm::vec2{0, newPos.y}, Pieces);
            Rook->setPos(glm::vec2{newPos.x+1, newPos.y});

        }
        hasMoved = true;
        return true;
    } else {
        return false;
    }
}

bool King::isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
    glm::vec2 kingPos;
    for (auto i : Pieces) {
        std::shared_ptr<King> derivedPtr = std::dynamic_pointer_cast<King>(i); 
        if (derivedPtr != nullptr && i->white == white) {
            kingPos = i->pos; 
        }
        else if (derivedPtr != nullptr && i->white != white) 
        {
            derivedPtr->isCastling = true;
            i->findMovesWithoutCheck(Pieces);
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
