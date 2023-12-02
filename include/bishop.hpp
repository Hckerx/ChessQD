#include "piece.hpp"

class Bishop : public Piece {
public:
    Bishop(glm::vec2 p_pos, bool white);

    void findMovesWithoutCheck(std::vector <std::shared_ptr<Piece>> &Pieces) override;

    SDL_Texture *textTexture;
    SDL_Color textColor;
private:
};
