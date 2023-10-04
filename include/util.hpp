#include "piece.hpp"

std::shared_ptr<Piece> getMatchingPiece(std::array<int, 2> field, std::vector<std::shared_ptr<Piece>>& Pieces);
std::array<float, 2> getMousePosition(bool whiteDown, int squareSize);
