#include "piece.hpp"
class Rook: public Piece
{
public:
    Rook(std::array<int, 2> p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces) ;
    bool move(std::array<int, 2> newPos, std::array<int, 2> oldPos, std::vector<std::shared_ptr<Piece>>& Pieces, bool whiteTurn, bool isPlayingOnline, bool isWhite);
    bool hasMoved = true;
;private:
};
