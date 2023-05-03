#include "util.hpp"
#include <cctype>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"

#include "pawn.hpp"
#include "rook.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "knight.hpp"
#include "Entity.hpp"

std::vector<Entity> FenImport(std::string FenString) {
  std::vector<Entity> Pieces;
  bool white_to_move;
  int countx = 0;
  int county = 0;
  for (char c: FenString) {
    if (std::isdigit(c)) {
      countx += atoi(&c);
    } else if (c == '/'){
      county += 1;
      countx = 0;
    } 
    else if (std::isalpha(c)){
      switch (tolower(c)){
        case 'k':
        Pieces.push_back(King({countx, county}, isupper(c)));
        countx += 1;
        break;
        case 'n':
        Pieces.push_back(Knight({countx, county}, isupper(c)));
        countx += 1;
        break;
        case 'p':
        Pieces.push_back(Pawn({countx, county}, isupper(c)));
        countx += 1;
        break;
        case 'r':
        Pieces.push_back(Rook({countx, county}, isupper(c)));
        countx += 1;
        break;
        case 'b':
        Pieces.push_back(Bishop({countx, county}, isupper(c)));
        countx += 1;
        break;
        case 'q':
        Pieces.push_back(Queen({countx, county}, isupper(c)));
        countx += 1;
        break;
      }
    }


    }
  return Pieces; 
}   
   
void selectPiece(int x, int y, std::vector<Entity>* Pieces) {
    // 
    // while () {
    //    
    //}
    return;
     
}



