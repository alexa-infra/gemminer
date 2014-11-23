#include "game/game.h"
#include "game/resource_manager.h"
#include "game/render_manager.h"
#include "game/board.h"
#include "SDL.h"

using namespace game;
using namespace base;

Game::Game()
{
    ResourceManager::init(this);
    RenderManager::init(this);
    RenderLayer* backgroundLayer = RenderManager::instance().addLayer("background", 1);
    background_ = new Sprite;
    background_->init("data/BackGround.jpg");
    backgroundLayer->sprites.push_back(background_);

    board_ = new Board(8, 8);
}

Game::~Game()
{
    delete background_;
    delete board_;
    RenderManager::shutdown();
    ResourceManager::shutdown();
}

void Game::OnFrame(float dt)
{
    Update(dt);
    Render();
    //SDLApp::OnFrame(dt);
}

void Game::Update(float dt)
{
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer_, NULL);

    RenderManager::instance().Render();

    SDL_RenderPresent(renderer_);
}

SDL_Renderer* Game::renderer()
{
    return renderer_;
}
