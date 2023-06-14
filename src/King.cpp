#include "king.hpp"
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
        // findIndMoves(Pieces, pos[0] + 1, pos[1]);
        // findIndMoves(Pieces, pos[0] - 1, pos[1]);
        std::shared_ptr<Piece> pieceTemp;
        std::shared_ptr<Rook> rookPiece;
        //if (findIndMoves(Pieces, pos[0] - 1, pos[1]) )
        //{
        //        if (!hasMoved)
        //        {

        //                pieceTemp = getMatchingPiece(glm::vec2{pos[0] - 4, pos[1]}, Pieces);
        //                rookPiece = std::dynamic_pointer_cast<Rook>(pieceTemp);
        //                if (rookPiece != nullptr)
        //                {
        //                      
        //                      if (findIndMoves(Pieces, pos[0] - 2, pos[1]))
        //                      {
        //                       
        //                        if (!rookPiece->hasMoved)
        //                        {
        //                                rookPiece->findMovesWithoutCheck(Pieces);
        //                                for (auto i : rookPiece->legalMoves)
        //                                {
        //                                        if (i == glm::vec2{pos[0] - 3, pos[1]})
        //                                        {
        //                                                legalMoves.push_back(glm::vec2{pos[0] - 2, pos[1]});
        //                                        }
        //                                }
        //                        }
        //                }}
        //        }
        //}
        //if (findIndMoves(Pieces, pos[0] + 1, pos[1]) )
        //{
        //       if (!hasMoved)
        //       {
        //        pieceTemp = getMatchingPiece(glm::vec2{pos[0] + 3 , pos[1]}, Pieces);
        //                rookPiece = std::dynamic_pointer_cast<Rook>(pieceTemp);
        //               if (rookPiece != nullptr){

        //                        if(findIndMoves(Pieces, pos[0] + 2, pos[1])){
        //                      if (!rookPiece->hasMoved)
        //                      {
        //                        legalMoves.push_back(glm::vec2{pos[0] + 2, pos[1]});
        //                      }
        //                      
        //               }}
        //       }
        //}
}

bool King::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn) {
        if (Piece::move(newPos, oldPos, Pieces, whiteTurn)) {
                hasMoved = true;
                return true;
        } else {
                return false;
        }
}
