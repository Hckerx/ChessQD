#include "util.hpp"
#include <cctype>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "renderWindow.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "piece.hpp"

std::vector<std::shared_ptr<Piece>> FenImport(std::string FenString) {
    std::cout << FenString << std::endl;
    std::vector<std::shared_ptr<Piece>> Pieces;
    int countx = 0;
    int county = 0;
    for (char c : FenString) {
        if (std::isdigit(c)) {
            int i = c - '0';
            countx += i;
        } else if (c == '/') {
            county += 1;
            countx = 0;
        } else if (std::isalpha(c)) {
            switch (tolower(c)) {
                case 'k':
                    Pieces.push_back(std::make_shared<King>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'n':
                    Pieces.push_back(std::make_shared<Knight>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'p':
                    Pieces.push_back(std::make_shared<Pawn>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'r':
                    Pieces.push_back(std::make_shared<Rook>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'b':
                    Pieces.push_back(std::make_shared<Bishop>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
                case 'q':
                    Pieces.push_back(std::make_shared<Queen>(glm::vec2{countx, county}, isupper(c)));
                    countx += 1;
                    break;
            }
        }
    }
    return Pieces;
}
// std::vector<std::shared_ptr<Piece>> FenImport(std::string FenString) {
//   std::vector<std::shared_ptr<Piece>> Pieces;
//   int countx = 0;
//   int county = 0;
//   for (char c: FenString) {
//     if (std::isdigit(c)) {
//       countx += atoi(&c);
//     } else if (c == '/'){
//       county += 1;
//       countx = 0;
//     } 
//     else if (std::isalpha(c)){
//       switch (tolower(c)){
//         case 'k':
//         Pieces.push_back(new King({countx, county}, isupper(c)));
//         countx += 1;
//         break;
//         case 'n':
//         Pieces.push_back (new Knight({countx, county}, isupper(c)));
//         countx += 1;
//         break;
//         case 'p':
//         Pieces.push_back(new Pawn({countx, county}, isupper(c)));
//         countx += 1;
//         break;
//         case 'r':
//         Pieces.push_back(new Rook({countx, county}, isupper(c)));
//         countx += 1;
//         break;
//         case 'b':
//         Pieces.push_back(new Bishop({countx, county}, isupper(c)));
//         countx += 1;
//         break;
//         case 'q':
//         Pieces.push_back(new Queen({countx, county}, isupper(c)));
//         countx += 1;
//         break;
//       }
//     }
//   }
//   return Pieces; 
// }   
   
std::shared_ptr<Piece> getMatchingPiece(glm::vec2 field, std::vector<std::shared_ptr<Piece>>& Pieces) {
    
    for (auto& i: Pieces)
    {
       if (field == i->pos){
          return i;
       }
    }  
    return nullptr;
}

glm::vec2 getMousePosition(bool whiteDown,int squareSize){
    int Mouse_x, Mouse_y;
    float Mousex, Mousey;
    SDL_GetMouseState(&Mouse_x, &Mouse_y);


    if(whiteDown){
        Mousex =  (float)Mouse_x/(float)squareSize;
        Mousey =  (float)Mouse_y/(float)squareSize;
        Mouse_x = Mouse_x/squareSize;
        Mouse_y = Mouse_y/squareSize;

    }
    else
    {
        Mousex =  8-(float)Mouse_x/(float)squareSize;
        Mousey =  8-(float)Mouse_y/(float)squareSize; 
        Mouse_x = 8-Mouse_x/squareSize;
        Mouse_y = 8-Mouse_y/squareSize;
    }
    return glm::vec2 {Mousex,Mousey};
}

