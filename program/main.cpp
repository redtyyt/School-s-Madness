#include <iostream>

#include "Game.h"

// using namespace sf;

int main(int argc, char const *argv[])
{
    Game game;

    while (game.running())
    {
        game.update();
        game.render();
    }


    return 0;
}
