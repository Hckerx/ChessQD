#include <string>
#include <vector>
#include "entity.hpp"
std::vector<Entity*> FenImport(std::string);
Entity* getMatchingPiece(int x, int y, std::vector<Entity*> Pieces);
