#pragma once

#include "base/sdlapp.h"

namespace game {
class Board;
struct Sprite;
class TimerDisplay;

namespace GameStates
{
enum GameState
{
    Start,
    Play,
    End
};
}
typedef GameStates::GameState GameState;

class Game : public ::base::SDLApp
{
public:
    Game();
    ~Game();

    SDL_Renderer* renderer();
protected:
    void Update(float dt);
    void Render();
    virtual void OnFrame(float dt);
    virtual void OnMouseUp(int x, int y);
    virtual void OnMouseDown(int x, int y);

    Sprite* background_;
    Board* board_;
    TimerDisplay* timer_;
    GameState state_;
};

}