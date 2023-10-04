#include "piece.hpp"
class Queen: public Piece
{
public:
    Queen(std::array<int, 2> p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces); 
private:
};
