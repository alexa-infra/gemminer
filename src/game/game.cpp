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
    ResourceManager::init<Game*>(this);
    RenderManager::init<Game*>(this);
    AnimationManager::init();
    RenderLayer* backgroundLayer = RenderManager::instance().AddLayer("background", 1);
    background_ = new Sprite;
    background_->Init("BackGround.jpg");
    backgroundLayer->sprites.push_back(background_);
    font = ResourceManager::instance().Font("agencyb.ttf");

    board_ = new Board(8, 8);
    timer_ = new TimerDisplay(60.f);
    state_ = GameStates::Start;
}

Game::~Game()
{
    delete background_;
    delete board_;
    delete timer_;
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
    AnimationManager::instance().Update(dt);

    if (state_ == GameStates::Start) {
    }
    else if (state_ == GameStates::Play) {
        board_->Update(dt);
        timer_->Update(dt);
        if (timer_->Finished() && AnimationManager::instance().Empty()) {
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
        rect.x = 280;
        rect.y = 255;
        rect.w = 190;
        rect.h = 70;
        SDL_RenderFillRect(renderer_, &rect);

        font->RenderText("Touch to Start", 300, 300);
    }
    else if (state_ == GameStates::Play) {
        font->RenderText(timer_->GetText(), 100, 100);
        font->RenderText(board_->GetScoresText(), 100, 150);
    }
    else if (state_ == GameStates::End) {
        SDL_Rect rect;
        rect.x = 240;
        rect.y = 255;
        rect.w = 280;
        rect.h = 105;
        SDL_RenderFillRect(renderer_, &rect);

        font->RenderText("Game finished!\nYour score is " + board_->GetScoresText(), 300, 300);
    }

    SDL_RenderPresent(renderer_);
}

SDL_Renderer* Game::GetRenderer()
{
    return renderer_;
}

void Game::OnMouseUp(int x, int y)
{
    if (state_ == GameStates::Start) {
        state_ = GameStates::Play;
        timer_->Reset(60.0f);
        board_->Fill();
    }
    else if (state_ == GameStates::Play) {
        board_->MouseUp(x, y);
    }
    else if (state_ == GameStates::End) {
        state_ = GameStates::Start;
    }
}

void Game::OnMouseDown(int x, int y)
{
    if (state_ == GameStates::Play) {
        board_->MouseDown(x, y);
    }
}
