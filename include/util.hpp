#include "piece.hpp"

std::shared_ptr <Piece> getMatchingPiece(glm::vec2 field, std::vector <std::shared_ptr<Piece>> &Pieces);

glm::vec2 getMousePosition(bool whiteDown, int squareSize);

std::string FenExport(const std::vector <std::shared_ptr<Piece>>& pieces,bool whiteTurn,int halfMoveNumber);

