#include "sdlapp.h"

#ifdef OS_WIN
#  define SDL_MAIN_HANDLED
#endif

#include <SDL.h>

#include <iostream>
#include <string>

namespace base {


SDLApp::SDLApp()
    : mainwindow_(nullptr)
    , renderer_(nullptr)
    , run_(true)
    , width_(755)
    , height_(600)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

    mainwindow_ = SDL_CreateWindow(
                      "GemMiner",
                      SDL_WINDOWPOS_CENTERED,
                      SDL_WINDOWPOS_CENTERED,
                      width_, height_,
                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                  );
    renderer_ = SDL_CreateRenderer(mainwindow_, -1, SDL_RENDERER_ACCELERATED);

    SDL_GL_SetSwapInterval( 1 );
}

SDLApp::~SDLApp()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow( mainwindow_ );
    SDL_Quit();
}

void SDLApp::PumpEvents()
{
    SDL_PumpEvents();
    SDL_Event event;

    while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_QUIT:
            run_ = false;
            break;
        case SDL_MOUSEBUTTONUP:
            OnMouseUp(event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            OnMouseDown(event.button.x, event.button.y);
            break;
        case SDL_MOUSEMOTION:
            OnMouseMove(event.motion.x,
                event.motion.y,
                event.motion.xrel,
                event.motion.yrel);

            break;
        case SDL_KEYDOWN:
            OnKeyboardDown( ( u8 )event.key.keysym.sym );
            break;
        case SDL_KEYUP:
            OnKeyboardUp( ( u8 )event.key.keysym.sym );
            break;
        default:
            break;
        }
    }
}

void SDLApp::Run()
{
    timer_.reset();

    while( run_ ) {
        PumpEvents();
        float dt = timer_.reset() / 1000.f;
        OnFrame(dt);
    }
}

void SDLApp::OnFrame(float dt)
{
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
}

void SDLApp::OnMouseMove( i32 x, i32 y, i32 dx, i32 dy )
{
}

void SDLApp::OnReshape( i32 width, i32 height )
{
}

void SDLApp::OnKeyboardDown( u8 key )
{   
}

void SDLApp::OnKeyboardUp( u8 key )
{
}

void SDLApp::OnMouseDown(i32 x, i32 y)
{

}

void SDLApp::OnMouseUp(i32 x, i32 y)
{

}

} // namespace base
