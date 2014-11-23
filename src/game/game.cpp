#include "game/game.h"
#include "game/resource_manager.h"
#include "game/render_manager.h"
#include "game/animation_manager.h"
#include "game/board.h"
#include "SDL.h"
#include "game/sprite_font.h"

using namespace game;
using namespace base;

static SpriteFont* font = nullptr;

Game::Game()
{
    ResourceManager::init(this);
    RenderManager::init(this);
    AnimationManager::init();
    RenderLayer* backgroundLayer = RenderManager::instance().addLayer("background", 1);
    background_ = new Sprite;
    background_->init("data/BackGround.jpg");
    backgroundLayer->sprites.push_back(background_);
    font = new SpriteFont(renderer_, "data/agencyb.ttf");

    board_ = new Board(8, 8);
}

Game::~Game()
{
    delete background_;
    delete board_;
    AnimationManager::shutdown();
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
    AnimationManager::instance().update(dt);
    board_->update(dt);
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer_, NULL);

    RenderManager::instance().Render();

    font->RenderText("hello world", 100, 100);

    SDL_RenderPresent(renderer_);
}

SDL_Renderer* Game::renderer()
{
    return renderer_;
}

void Game::OnMouseUp(int x, int y)
{
    board_->click(x, y);
}
