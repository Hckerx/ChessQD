#include <string>
#include <vector>
#include <memory>
#include "piece.hpp"

// Inputs

//std::vector<std::shared_ptr<Piece>> FenImport(std::string);
std::vector<std::shared_ptr<Piece>> FenImport(std::string FenString);
std::shared_ptr<Piece> getMatchingPiece(glm::vec2 field, std::vector<std::shared_ptr<Piece>>& Pieces);
glm::vec2 getMousePosition(bool whiteDown, int squareSize);
