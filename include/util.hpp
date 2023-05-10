#include <string>
#include <vector>
#include "Entity.hpp"
std::vector<Entity*> FenImport(std::string);
Entity* selectPiece(int x, int y, std::vector<Entity*> Pieces);
