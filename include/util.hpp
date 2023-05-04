#include <string>
#include <vector>
#include "Entity.hpp"
std::vector<Entity*> FenImport(std::string);
glm::vec2 selectPiece(int x, int y, std::vector<Entity*> Pieces);
