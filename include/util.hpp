#include <string>
#include <vector>
#include <memory>
#include "entity.hpp"

// Inputs

//std::vector<std::shared_ptr<Entity>> FenImport(std::string);
std::vector<std::shared_ptr<Entity>> FenImport(std::string FenString);
std::shared_ptr<Entity> getMatchingPiece(glm::vec2 field, std::vector<std::shared_ptr<Entity>>& Pieces);
glm::vec2 getMousePosition(bool rotating, int squareSize);
