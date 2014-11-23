#include "game/game.h"
#include "game/resource_manager.h"
#include "game/render_manager.h"
#include "game/animation_manager.h"
#include "game/board.h"
#include "SDL.h"
#include "game/sprite_font.h"
#include "game/timer_display.h"

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
    timer_ = new TimerDisplay(60.f);
    state_ = GameStates::Start;
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
    if (state_ == GameStates::Start) {
        timer_->reset(60.0f);
    }
    else if (state_ == GameStates::Play) {
        AnimationManager::instance().update(dt);
        board_->update(dt);
        timer_->update(dt);
        if (timer_->isFinished()) {
            state_ = GameStates::End;
        }
    }
    else if (state_ == GameStates::End) {
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer_, NULL);

    RenderManager::instance().Render();

    if (state_ == GameStates::Start) {
        SDL_Rect rect;
        rect.x = 280; rect.y = 255;
        rect.w = 190; rect.h = 70;
        SDL_RenderFillRect(renderer_, &rect);

        font->RenderText("Touch to Start", 300, 300);
    }
    else if (state_ == GameStates::Play) {
        font->RenderText(timer_->getText(), 100, 100);
        font->RenderText(board_->getScoresText(), 100, 150);
    }
    else if (state_ == GameStates::End) {
        SDL_Rect rect;
        rect.x = 240; rect.y = 255;
        rect.w = 280; rect.h = 105;
        SDL_RenderFillRect(renderer_, &rect);

        font->RenderText("Game finished!\nYour score is " + board_->getScoresText(), 300, 300);
    }

    SDL_RenderPresent(renderer_);
}

SDL_Renderer* Game::renderer()
{
    return renderer_;
}

void Game::OnMouseUp(int x, int y)
{
    if (state_ == GameStates::Start) {
        state_ = GameStates::Play;
    }
    else if (state_ == GameStates::Play) {
        board_->click(x, y);
    }
    else if (state_ == GameStates::End) {
        state_ = GameStates::Start;
    }
}
