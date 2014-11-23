#pragma once

#include "base/sdlapp.h"

namespace game {
    
    class Game : public ::base::SDLApp
    {
    public:
        Game();
    protected:
        void Update(float dt);
        void Render();
        virtual void OnFrame(float dt);
    };

}