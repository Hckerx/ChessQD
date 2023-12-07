#include "piece.hpp"
#include "util.hpp"
#include <algorithm>
#include <king.hpp>
#include <pawn.hpp>

/**
* constructor for the piece
* @param p_pos the position of the piece
* @param white the color of the piece
*/
Piece::Piece(glm::vec2 p_pos, bool white)
        : pos(p_pos), white(white) {
}

/**
* get the position and size of the piece on the sprite
* @return SDL_Rect the position and size of the piece on the sprite
*/
SDL_Rect Piece::getCurrentFrame() const {
    return currentFrame;
}

/**
* Function to get individual moves of the piece
* by checking if the position is occupied by a piece of the same color and if the position is on the board
* @param Pieces a vector containing all pieces
* @param x the x coordinate of the position to check
* @param y the y coordinate of the position to check
* @return bool true if the position is empty useful for rook queen and bishop becuase if false they cant move further
*/
bool Piece::findIndMoves(std::vector <std::shared_ptr<Piece>> &Pieces, float x, float y) {
    std::shared_ptr <Piece> hypotheticalPiece = getMatchingPiece(glm::vec2{x, y}, Pieces);
    if (x > 7 || x < 0 || y > 7 || y < 0) {
        return false;
    }
    if (hypotheticalPiece == nullptr) {
        legalMoves.emplace_back(x, y);
        return true;
    } else if (hypotheticalPiece->white == white) {
        return false;
    } else {
        legalMoves.emplace_back(x, y);
        return false;
    }
}

/**
* Default function to move a piece
* @param newPos the position to move to
* @param oldPos the position to move from
* @param Pieces a vector containing all pieces
* @param whiteTurn true if it is the white players turn
* @param isPlayingOnline true if the game is played online
* @param isWhite true if the player is white
* @return true if the move was successful
*/

bool Piece::move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
                 bool isPlayingOnline, bool isWhite) {
    // check for correct turn and online correct turn
    if (whiteTurn == white && (!isPlayingOnline || isWhite == whiteTurn)) {
        for (glm::vec2 i: legalMoves) {
            if (i == newPos) {
                // if piece moves to a position occupied by an enemy piece delete the enemy piece
                std::shared_ptr <Piece> hypoPiece = getMatchingPiece(glm::vec2{newPos.x, newPos.y}, Pieces);
                if (hypoPiece != nullptr) {
                    auto position = std::find(Pieces.begin(), Pieces.end(), hypoPiece);
                    if (position != Pieces.end()) { // == myVector.end() means the element was not found
                        Pieces.erase(position);
                    }
                }

                setPos(newPos);


                // since piece has moved no pawn can be vulnerable to en passant anymore
                for (auto &i: Pieces) {
                    std::shared_ptr <Pawn> derivedPtr = std::dynamic_pointer_cast<Pawn>(i);
                    if (derivedPtr != nullptr) {
                        derivedPtr->isEnPassantVulnerable = false;
                    }
                }
                return true;

            }
        }
    }
    setPos(oldPos);
    return false;
}
/**
* general function to move pieces
* @param Pieces a vector containing all pieces
* @return true if piece has no legal moves
*/
bool Piece::findMoves(std::vector <std::shared_ptr<Piece>> &Pieces) {
    float step;
    if (white) {
        step = 1;
    } else {
        step = -1;
    }
    std::shared_ptr <Piece> hypoPiece;
    findMovesWithoutCheck(Pieces);
    std::vector <glm::vec2> legalMovescopy = legalMoves;
    std::vector <glm::vec2> newLegalMoves;

    glm::vec2 oldpos = pos;

    glm::vec2 hypoCoords;
    for (auto move: legalMovescopy) {
        hypoPiece = getMatchingPiece(move, Pieces);
        // if the piece is a pawn check if it is an en passant move
        if (hypoPiece == nullptr) {
            if (typeid(*this) == typeid(Pawn)) {
                hypoPiece = getMatchingPiece(glm::vec2{move.x, move.y + step}, Pieces);
            }
        }
        // check if taking the move would result in the king being in check
        if (hypoPiece != nullptr) {
            hypoCoords = hypoPiece->getPos(); // No one ever should resort to such a dirty trick but it works fuck it
            hypoPiece->setPos({2000, 2000});
            pos = move; //neue position setzten um damit die moves alleer pieces zu überprüfen
            if (!isKingInCheck(Pieces)) {
                newLegalMoves.push_back(move);
            }
            hypoPiece->setPos(hypoCoords);

        } else {
            // check if moving without taking a piece results in check
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

/**
* function to check if the king is in check
* @param Pieces a vector containing all pieces
* @return true if king is in check
*/
bool Piece::isKingInCheck(std::vector <std::shared_ptr<Piece>> &Pieces) const {
    glm::vec2 kingPos;
    for (const auto& i: Pieces) {
        std::shared_ptr <King> derivedPtr = std::dynamic_pointer_cast<King>(i); //was macht das?
        if (derivedPtr != nullptr && i->white == white) {
            kingPos = i->pos;
        } else {
            i->findMovesWithoutCheck(Pieces);
        }

    }
    // for piece in pieces
    //     piece.findmoves

    for (auto &i: Pieces) {
        if (i->white != white) {
            for (glm::vec2 j: i->legalMoves) {
                if (j == kingPos) {
                    return true;
                }
            }
        }
    }
    return false;
}
