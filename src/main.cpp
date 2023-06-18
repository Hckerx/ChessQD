#include "game.hpp"

// TODO Schach, En passant, checkmate, rotating of table, online, potentially ai????????
int main(int argc, char* args[])
{
    std::string input;
    std::getline(std::cin,input);
    Game Game(input);
    return 0;
}
