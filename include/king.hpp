#include "piece.hpp"

class King : public Piece {
public:
    King(glm::vec2 p_pos, bool white);

    bool isKingInCheck(std::vector <std::shared_ptr<Piece>> &Pieces);

    void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) override;

    bool move(glm::vec2 newPos, glm::vec2 oldPos, std::vector <std::shared_ptr<Piece>> &Pieces, bool whiteTurn,
              bool isPlayingOnline, bool isWhite) override;

    bool hasMoved = false;
    bool isCastling = false;
    bool canCastleKing = true;
    bool canCastleQueen = true;
};

