#include "rook.hpp"
#include "entity.hpp"
#include "SDL2/SDL.h"


#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "util.hpp"
#include <glm/fwd.hpp>
#include <iostream>
#include <vector>
Rook::Rook(glm::vec2 p_pos, bool white)
:Entity(p_pos, white){
        if (white) {
                currentFrame.y = 128;
        }
        else {
                currentFrame.y = 0;
        }
        currentFrame.x = 128*3;
        currentFrame.w = 128;
        currentFrame.h = 128;
}

void Rook::findMoves(std::vector<Entity *> Pieces) {
        legalMoves.clear();
        // erst x
        int pos_x = pos[0]+1;
            for (int8_t i = pos_x; i < 8; i++)
            {
                std::cout << i << std::endl;
                Entity* hypotheticalPiece = getMatchingPiece(i, pos[1], Pieces);
                    if (hypotheticalPiece == nullptr) {
                        legalMoves.push_back(glm::vec2(i,pos[1]));
                    }
                    else if (hypotheticalPiece->white == white) {
                            break;
                    }
                    else {
                        legalMoves.push_back(glm::vec2(i,pos[1]));
                        break;
                    }
            }
        pos_x = pos[0]-1;
            for (int8_t i = pos_x; i >= 0; i--)
            {
                Entity* hypotheticalPiece = getMatchingPiece(i, pos[1], Pieces);
                    if (hypotheticalPiece == nullptr) {
                        legalMoves.push_back(glm::vec2(i,pos[1]));
                    }
                    else if (hypotheticalPiece->white == white) {
                            break;
                    }
                    else {
                        legalMoves.push_back(glm::vec2(i,pos[1]));
                        break;
                    }
            }
        int pos_y = pos[1]+1;
            for (int8_t i = pos_y; i < 8; i++)
            {
                    std::cout << i << std::endl;
                Entity* hypotheticalPiece = getMatchingPiece(pos[0], i, Pieces);
                    if (hypotheticalPiece == nullptr) {
                        legalMoves.push_back(glm::vec2(pos[0],i));
                    }
                    else if (hypotheticalPiece->white == white) {
                            break;
                    }
                    else {
                        legalMoves.push_back(glm::vec2(pos[0],i));
                        break;
                    }
            }
        pos_y = pos.y - 1;
        for (int8_t i = pos_y; i >= 0; i-=1)
        {
            Entity* hypotheticalPiece = getMatchingPiece(pos[0], i, Pieces);
                if (hypotheticalPiece == nullptr) {
                    legalMoves.push_back(glm::vec2(pos[0], i));
                }
                else if (hypotheticalPiece->white == white) {
                        break;
                }
                else {
                    legalMoves.push_back(glm::vec2(pos[0], i));
                    break;
                }
        }
    for (glm::vec2 i: legalMoves) {
        std::cout << glm::to_string(i) << std::endl;
    }
}
