#include "king.hpp"
#include "piece.hpp"
#include "util.hpp"
#include "rook.hpp"

#include <iostream>
King::King(std::array<int, 2> p_pos, bool white)
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

void King::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>> &Pieces) {
    // FIXME add special Rules rochade
    legalMoves.clear();
    findIndMoves(Pieces, pos[0] + 1, pos[1] + 1);
    findIndMoves(Pieces, pos[0] + 1, pos[1] - 1);
    findIndMoves(Pieces, pos[0] - 1, pos[1] + 1);
    findIndMoves(Pieces, pos[0] - 1, pos[1] - 1);
    findIndMoves(Pieces, pos[0], pos[1] - 1);
    findIndMoves(Pieces, pos[0], pos[1] + 1);
    // prevent recursion??

    // For Castling Only
    if (findIndMoves(Pieces, pos[0] - 1, pos[1]) && !hasMoved) {
        pos[0] = pos[0] - 1;
        if (!isKingInCheck(Pieces)) {
            pos[0] = pos[0] - 2;
            if (!isKingInCheck(Pieces)) {
                pos[0] = pos[0] + 3;

                std::shared_ptr<Piece> piece =
                getMatchingPiece(std::array<int, 2>{pos[0] - 4, pos[1]}, Pieces);
                std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(piece);
                if (rook != nullptr) {
                    if (!rook->hasMoved) {
                        rook->findMovesWithoutCheck(Pieces);
                        for (auto i : rook->legalMoves) {
                            if (i == std::array<int, 2>{pos[0] - 3, pos[1]}) {
                                findIndMoves(Pieces, pos[0] - 2, pos[1]);
                            }
                        }
                    }
                }
            } else {
                pos[0] = pos[0] + 3;
            }
        } else {
            pos[0] = pos[0] + 1;
        }
    }

    if (findIndMoves(Pieces, pos[0] + 1, pos[1]) && !hasMoved) {
        pos[0] = pos[0] + 1;
        if (!isKingInCheck(Pieces)) {
            pos[0] = pos[0] - 1;
            std::shared_ptr<Piece> piece =
            getMatchingPiece(std::array<int, 2>{pos[0] + 3, pos[1]}, Pieces);
            std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(piece);
            if (rook != nullptr) {
                if (!rook->hasMoved) {
                    findIndMoves(Pieces, pos[0] + 2, pos[1]);
                }
            }
        }   else {
            pos[0] = pos[0] - 1;
        }
    }
}

//Special move function for castling
bool King::move(std::array<int, 2> newPos, std::array<int, 2> oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn, bool isPlayingOnline, bool isWhite) {
    if (Piece::move(newPos, oldPos, Pieces, whiteTurn, isPlayingOnline, isWhite)) {
        if (newPos[0] - oldPos[0] == 2) {
            std::shared_ptr<Piece> Rook = getMatchingPiece(std::array<int, 2>{7, newPos[1]}, Pieces);
            Rook->setPos(std::array<int, 2>{5, newPos[1]});
        }
        if (oldPos[0] - newPos[0] == 2) {
            std::shared_ptr<Piece> Rook = getMatchingPiece(std::array<int, 2>{0, newPos[1]}, Pieces);
            Rook->setPos(std::array<int, 2>{newPos[0]+1, newPos[1]});

        }
        hasMoved = true;
        return true;
    } else {
        return false;
    }
}

bool King::isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
    std::array<int, 2> kingPos;
    for (auto i : Pieces) {
        std::shared_ptr<King> derivedPtr = std::dynamic_pointer_cast<King>(i); 
        if (derivedPtr != nullptr && i->white == white) {
            kingPos = i->pos; 
        }
        // else if (derivedPtr != nullptr && i->white != white) 
        // {
        //     derivedPtr->isCastling = true;
        //     i->findMovesWithoutCheck(Pieces);
        // }
 
    }
    // for piece in pieces
    //     piece.findmoves

    for (auto& i : Pieces) {
        if (i->white != white) {
            for (std::array<int, 2> j : i->legalMoves) {
                if (j == pos) { //kingPos
                    return true;
                }
            }    
        }
    }
    return false;
}
