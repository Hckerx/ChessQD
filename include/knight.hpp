#include "piece.hpp"
class Knight: public Piece
{
public:
    Knight(std::array<int, 2> p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces);

private:
};
