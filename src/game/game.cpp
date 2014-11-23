#include "game/game.h"
#include "game/resource_manager.h"
#include "SDL.h"

using namespace game;
using namespace base;

static SDL_Texture* tttt = nullptr;

Game::Game()
{
    ResourceManager::init(this);
    tttt = ResourceManager::instance().Texture("data/BackGround.jpg");
}

Game::~Game()
{
    ResourceManager::shutdown();
}

void Game::OnFrame(float dt)
{
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer_, NULL);
    SDL_RenderCopy(renderer_, tttt, NULL, NULL);
    SDL_RenderPresent(renderer_);
    //SDLApp::OnFrame(dt);
}

void Game::Update(float dt)
{
}

void Game::Render()
{
}

SDL_Renderer* Game::renderer()
{
    return renderer_;
}
