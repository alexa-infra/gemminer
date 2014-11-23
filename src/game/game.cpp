#include "game/game.h"
#include "game/resource_manager.h"
#include "game/render_manager.h"
#include "SDL.h"

using namespace game;
using namespace base;

static Sprite* tttt = nullptr;

Game::Game()
{
    ResourceManager::init(this);
    RenderManager::init(this);
    RenderLayer* backgroundLayer = RenderManager::instance().addLayer("background", 1);
    tttt = new Sprite;
    tttt->init("data/BackGround.jpg");
    backgroundLayer->sprites.push_back(tttt);
}

Game::~Game()
{
    RenderManager::shutdown();
    ResourceManager::shutdown();
}

void Game::OnFrame(float dt)
{
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer_, NULL);

    RenderManager::instance().Render();

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
