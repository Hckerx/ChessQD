#include "king.hpp"
#include "piece.hpp"
#include "util.hpp"
#include "rook.hpp"

#include <iostream>
King::King(glm::vec2 p_pos, bool white)
:Piece(p_pos, white){
    if (white)
        currentFrame.y = 128;
    else
        currentFrame.y = 0;

    currentFrame.x = 128*5;
    currentFrame.w = 128;
    currentFrame.h = 128;
}

void King::findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>> &Pieces) {
    legalMoves.clear();
    findIndMoves(Pieces, pos.x + 1, pos.y + 1);
    findIndMoves(Pieces, pos.x + 1, pos.y - 1);
    findIndMoves(Pieces, pos.x - 1, pos.y + 1);
    findIndMoves(Pieces, pos.x - 1, pos.y - 1);
    findIndMoves(Pieces, pos.x, pos.y - 1);
    findIndMoves(Pieces, pos.x, pos.y + 1);

    // For Castling long castle
    if (findIndMoves(Pieces, pos.x - 1, pos.y) && !hasMoved) {
        pos.x = pos.x - 1;
        if (!isKingInCheck(Pieces)) {
            pos.x = pos.x - 2;
            if (!isKingInCheck(Pieces)) {
                pos.x = pos.x + 3;

                std::shared_ptr<Piece> piece = getMatchingPiece(glm::vec2{pos.x - 4, pos.y}, Pieces);
                std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(piece);
                if (rook != nullptr) {
                    if (!rook->hasMoved) {
                        rook->findMovesWithoutCheck(Pieces);
                        for (auto i : rook->legalMoves) {
                            if (i == glm::vec2{pos.x - 3, pos.y}) {
                                findIndMoves(Pieces, pos.x - 2, pos.y);
                            }
                        }
                    }
                }
            } else
                pos.x = pos.x + 3;
        } else
            pos.x = pos.x + 1;
    }
// For Castling short castle
    if (findIndMoves(Pieces, pos.x + 1, pos.y) && !hasMoved) {
        pos.x = pos.x + 1;
        if (!isKingInCheck(Pieces)) {
            pos.x = pos.x - 1;
            std::shared_ptr<Piece> piece =
            getMatchingPiece(glm::vec2{pos.x + 3, pos.y}, Pieces);
            std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(piece);
            if (rook != nullptr) {
                if (!rook->hasMoved) {
                    findIndMoves(Pieces, pos.x + 2, pos.y);
                }
            }
        }   else {
            pos.x = pos.x - 1;
        }
    }
}

//Special move function for castling
bool King::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn, bool isPlayingOnline, bool isWhite) {
    if (Piece::move(newPos, oldPos, Pieces, whiteTurn, isPlayingOnline, isWhite)) {
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
    } else
        return false;
}
//looks for every move if King is in check
bool King::isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces) {
    for (auto& i : Pieces) {
        if (i->white != white) {
            for (glm::vec2 j : i->legalMoves) {
                if (j == pos) {
                    return true;
                }
            }    
        }
    }
    return false;
}