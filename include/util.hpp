#include "piece.hpp"

std::shared_ptr <Piece> getMatchingPiece(glm::vec2 field, std::vector <std::shared_ptr<Piece>> &Pieces);

glm::vec2 getMousePosition(bool whiteDown, int squareSize);

std::string FenExport(const std::vector <std::shared_ptr<Piece>>& piecesVector,bool whiteTurn,int halfMoveNumber);
std::vector<std::shared_ptr<Piece>> FenImport(const std::string& FenString);