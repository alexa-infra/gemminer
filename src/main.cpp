#include "game/game.h"
#include <SDL_main.h>

int main(int argc, char* argv[])
{
    using namespace game;
    Game app;
    app.Run();
    return 0;
}
