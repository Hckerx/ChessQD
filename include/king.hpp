#include "piece.hpp"
class King: public Piece
{
public:
    King(std::array<int, 2> p_pos, bool white);
    bool isKingInCheck(std::vector<std::shared_ptr<Piece>>& Pieces);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces); 
    bool move(std::array<int, 2> newPos, std::array<int, 2> oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn, bool isPlayingOnline, bool isWhite);
    bool hasMoved = false;
    bool isCastling = false;
    bool canCastleKing = true;
    bool canCastleQueen = true;
private:
};

