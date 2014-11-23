#pragma once

#include "base/sdlapp.h"

namespace game {
    class Board;
    struct Sprite;

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

        Sprite* background_;
        Board* board_;
    };

}