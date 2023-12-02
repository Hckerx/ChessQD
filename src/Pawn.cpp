#include "pawn.hpp"
#include "piece.hpp"
#include "game.hpp"
#include "util.hpp"

Pawn::Pawn(glm::vec2 p_pos, bool white)
        : Piece(p_pos, white) {
    //step defines the direction of movement
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

void Pawn::findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) {
    legalMoves.clear();

    if (getMatchingPiece(glm::vec2{pos.x, pos.y - step}, Pieces) == nullptr &&
        !(pos.x > 7 || pos.x < 0 || pos.y - step > 7 || pos.y - step < 0)) {
        legalMoves.push_back(glm::vec2(pos.x, pos.y - step));
        if (((pos.y == 6 && white) || pos.y == 1) &&
            getMatchingPiece(glm::vec2{pos.x, pos.y - 2 * step}, Pieces) == nullptr) {
            if (!(pos.x > 7 || pos.x < 0 || pos.y - 2 * step > 7 || pos.y - 2 * step < 0)) {
                legalMoves.emplace_back(pos.x, pos.y - 2 * step);
            }
        }
    }

    std::shared_ptr <Piece> hypoPiece = getMatchingPiece(glm::vec2{pos.x - 1, pos.y - step}, Pieces);
    if (hypoPiece != nullptr) {
        if (hypoPiece->white != white && !(pos.x - 1 > 7 || pos.x - 1 < 0 || pos.y - step > 7 || pos.y - step < 0)) {
            legalMoves.push_back(glm::vec2(pos.x - 1, pos.y - step));
        }
    }
    hypoPiece = getMatchingPiece(glm::vec2{pos.x + 1, pos.y - step}, Pieces);
    if (hypoPiece != nullptr) {
        if (hypoPiece->white != white && !(pos.x + 1 > 7 || pos.x + 1 < 0 || pos.y + step > 7 || pos.y + step < 0)) {
            legalMoves.push_back(glm::vec2(pos.x + 1, pos.y - step));
        }
    }
    hypoPiece = getMatchingPiece(glm::vec2{pos.x - 1, pos.y}, Pieces);
    std::shared_ptr <Pawn> hypoPawn = std::dynamic_pointer_cast<Pawn>(hypoPiece); // was macht das?
    if (hypoPawn != nullptr) {
        if (hypoPawn->isEnPassantVulnerable) {
            legalMoves.emplace_back(pos.x - 1, pos.y - step);
        }
    }

    hypoPiece = getMatchingPiece(glm::vec2{pos.x + 1, pos.y}, Pieces);
    hypoPawn = std::dynamic_pointer_cast<Pawn>(hypoPiece); // was macht das?
    if (hypoPawn != nullptr) {
        if (hypoPawn->isEnPassantVulnerable) {
            legalMoves.emplace_back(pos.x + 1, pos.y - step);
        }
    }
}


//NOTE: Since Pawn can only move diagonal when capturing overwrite of move function is necessary
bool Pawn::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
                bool isPlayingOnline, bool isWhite) {
    std::shared_ptr <Piece> hypoPiece;

    if (whiteTurn == white && (!isPlayingOnline || isWhite == whiteTurn)) {
        //if (whiteTurn == white) {
        for (glm::vec2 i: legalMoves) {
            if (i == newPos) {
                if ((white && oldPos.y == 6 && newPos.y == 4) || (!white && oldPos.y == 1 && newPos.y == 3)) {
                    isEnPassantVulnerable = true;
                }

                if (oldPos.x != newPos.x) {
                    hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y}, Pieces);
                    if (hypoPiece == nullptr) {
                        hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y + step}, Pieces);
                    }

                    auto position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()) { // == myVector.end() means the element was not found
                        Pieces.erase(position);
                    }

                }
                for (auto &i: Pieces) {
                    if (this == i.get()) {
                        continue;
                    }
                    std::shared_ptr <Pawn> hypoPawn = std::dynamic_pointer_cast<Pawn>(i);
                    if (hypoPawn != nullptr) {

                        hypoPawn->isEnPassantVulnerable = false;
                    }
                }
                setPos(newPos);
                return true;
            }
        }
        setPos(oldPos);
        return false;
    }
    setPos(oldPos);
    return false;
}
