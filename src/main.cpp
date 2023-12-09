#include "game.hpp"
#define BOOST_TEST_MODULE MyTest

/** main function which starts the game with the default fen
* @param void
* @return 0
*/
int main() {
    Game Game(defaultFen);
    return 0;
}
