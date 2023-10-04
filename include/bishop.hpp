#include "piece.hpp"
class Bishop: public Piece
{
public:
    Bishop(std::array<int, 2> p_pos, bool white);
    void findMovesWithoutCheck(std::vector<std::shared_ptr<Piece>>& Pieces); 
    SDL_Texture* textTexture;
    SDL_Color textColor;
private:
};
