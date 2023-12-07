#include "pawn.hpp"
#include "game.hpp"
#include "util.hpp"

/* constructor for the pawn
* define the pawns position on sprite in bin/debug/res/gfx/pieces.png
* inherit from piece
* @param p_pos the position of the pawn
* @param white the color of the pawn
*/
Pawn::Pawn(glm::vec2 p_pos, bool white)
        : Piece(p_pos, white) {
    //step defines the direction of movement based on the color of the pawn
    if (white) {
        currentFrame.y = 128;
        step = 1;
    } else {
        currentFrame.y = 0;
        step = -1;
    }
    currentFrame.x = 0;
    currentFrame.w = 128;
    currentFrame.h = 128;
}
/**
* finds all possible moves of the pawn
* @param Pieces a vector containing all pieces
*/
void Pawn::findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) {
    legalMoves.clear();

    // check if pawn can move forward
    // if pawn is on starting position it can move 2 fields forward
    if (getMatchingPiece(glm::vec2{pos.x, pos.y - step}, Pieces) == nullptr &&
        !(pos.x > 7 || pos.x < 0 || pos.y - step > 7 || pos.y - step < 0)) {
        legalMoves.emplace_back(pos.x, pos.y - step);
        if (((pos.y == 6 && white) || pos.y == 1) &&
            getMatchingPiece(glm::vec2{pos.x, pos.y - 2 * step}, Pieces) == nullptr) {
            if (!(pos.x > 7 || pos.x < 0 || pos.y - 2 * step > 7 || pos.y - 2 * step < 0)) {
                legalMoves.emplace_back(pos.x, pos.y - 2 * step);
            }
        }
    }
    // check if pawn can capture
    std::shared_ptr <Piece> hypoPiece = getMatchingPiece(glm::vec2{pos.x - 1, pos.y - step}, Pieces);
    if (hypoPiece != nullptr) {
        if (hypoPiece->white != white && !(pos.x - 1 > 7 || pos.x - 1 < 0 || pos.y - step > 7 || pos.y - step < 0)) {
            legalMoves.emplace_back(pos.x - 1, pos.y - step);
        }
    }
    // check if pawn can capture
    hypoPiece = getMatchingPiece(glm::vec2{pos.x + 1, pos.y - step}, Pieces);
    if (hypoPiece != nullptr) {
        if (hypoPiece->white != white && !(pos.x + 1 > 7 || pos.x + 1 < 0 || pos.y + step > 7 || pos.y + step < 0)) {
            legalMoves.emplace_back(pos.x + 1, pos.y - step);
        }
    }
    // check if pawn can capture en passant
    hypoPiece = getMatchingPiece(glm::vec2{pos.x - 1, pos.y}, Pieces);
    std::shared_ptr <Pawn> hypoPawn = std::dynamic_pointer_cast<Pawn>(hypoPiece); // was macht das?
    if (hypoPawn != nullptr) {
        if (hypoPawn->isEnPassantVulnerable) {
            legalMoves.emplace_back(pos.x - 1, pos.y - step);
        }
    }
    // check if pawn can capture en passant
    hypoPiece = getMatchingPiece(glm::vec2{pos.x + 1, pos.y}, Pieces);
    hypoPawn = std::dynamic_pointer_cast<Pawn>(hypoPiece); // was macht das?
    if (hypoPawn != nullptr) {
        if (hypoPawn->isEnPassantVulnerable) {
            legalMoves.emplace_back(pos.x + 1, pos.y - step);
        }
    }
}

/**
* moves the pawn
* @param newPos the new position of the pawn
* @param oldPos the old position of the pawn
* @param Pieces a vector containing all pieces
* @param whiteTurn a bool indicating if it is whites turn
* @param isPlayingOnline a bool indicating if the game is played online
* @param isWhite a bool indicating if the player is white
* @return true if the move was successful
*/
bool Pawn::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
                bool isPlayingOnline, bool isWhite) {
    std::shared_ptr <Piece> hypoPiece;

    // only move if it is the the correct turn even if playing online
    if (whiteTurn == white && (!isPlayingOnline || isWhite == whiteTurn)) {
        //if (whiteTurn == white) {
        for (glm::vec2 i: legalMoves) {
            // check if the new position is in the legal moves
            if (i == newPos) {
                // if pawn moved 2 fields it is vulnerable to en passant
                if ((white && oldPos.y == 6 && newPos.y == 4) || (!white && oldPos.y == 1 && newPos.y == 3)) {
                    isEnPassantVulnerable = true;
                }

                // check if pawn moved diagonally and if so if it captured a piece
                if (oldPos.x != newPos.x) {
                    hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y}, Pieces);
                    if (hypoPiece == nullptr) {
                        hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y + step}, Pieces);
                    }

                    // remove captured piece from vector
                    auto position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()) { // == myVector.end() means the element was not found
                        Pieces.erase(position);
                    }

                }
                // Since the pawn moved no pawn can be vulnerable to en passant anymore
                for (auto &i: Pieces) {
                    if (this == i.get()) {
                        continue;
                    }
                    std::shared_ptr <Pawn> hypoPawn = std::dynamic_pointer_cast<Pawn>(i);
                    if (hypoPawn != nullptr) {

                        hypoPawn->isEnPassantVulnerable = false;
                    }
                }
                // move the pawn
                setPos(newPos);
                return true;
            }
        }
        // move not possible
        // cancel move
        setPos(oldPos);
        return false;
    }
    // move not possible
    // cancel move
    setPos(oldPos);
    return false;
}
