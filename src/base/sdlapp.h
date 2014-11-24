#pragma once

#include "base/types.h"
#include "base/timer.h"

struct SDL_Window;
struct SDL_Renderer;

namespace base {

//! Shim for SDL functions and event loop
class SDLApp
{
public:
    SDLApp();
    virtual ~SDLApp();
    virtual void Run();
protected:
    virtual void OnFrame(float dt);
    virtual void OnMouseMove( i32 x, i32 y, i32 dx, i32 dy );
    virtual void OnMouseDown(i32 x, i32 y);
    virtual void OnMouseUp(i32 x, i32 y);
    void PumpEvents();
protected:
    SDL_Window* mainwindow_;
    SDL_Renderer* renderer_;
    bool run_;
    u32 width_;
    u32 height_;
    Timer timer_;
};

} // namespace base
