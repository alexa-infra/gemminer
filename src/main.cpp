#include "game/game.h"
#if defined(OS_MINGW)
    #define SDL_MAIN_HANDLED
#endif
#include "SDL_main.h"

int main(int argc, char* argv[])
{
    using namespace game;
    Game app;
    app.Run();
    return 0;
}
