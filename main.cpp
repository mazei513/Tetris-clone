#include "game.h"

int main(int argv, char* argc[])
{
    srand(time(NULL));

    cgame Game;

    bool quit = false;

    Game.init_board_graphics();

    while(!quit)
    {
        Game.handle_input(quit);

        Game.render();
    }

    return 0;
}
