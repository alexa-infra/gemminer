#pragma once

#include "base/types.h"

struct SDL_Window;
struct SDL_Renderer;

namespace base {

class SDLApp
{
public:
    SDLApp();
    virtual ~SDLApp();
    virtual void Run();
protected:
    virtual void OnFrame();
    virtual void OnMouseMove( i32 x, i32 y, i32 dx, i32 dy );
    virtual void OnReshape( i32 width, i32 height );
    virtual void OnKeyboardDown( u8 key );
    virtual void OnKeyboardUp( u8 key );
    void Pump();
protected:
    SDL_Window* mainwindow_;     //!< window handle
    SDL_Renderer* renderer_;
    void* maincontext_;          //!< GL context
    bool run_;
    u32 width_;
    u32 height_;
};

} // namespace base
