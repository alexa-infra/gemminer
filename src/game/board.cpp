#include "game/board.h"
#include "game/render_manager.h"
#include "game/animation_manager.h"
#include <cmath>
#include <string>
#include <sstream>

using namespace game;
using namespace base;

const int spriteCount = 5;
std::string spriteNames[spriteCount] = {
    "Blue.png",
    "Green.png",
    "Purple.png",
    "Red.png",
    "Yellow.png"
};

// Animation durations
const float Animation_Appear = 0.5f;
const float Animation_Move = 0.25f;
const float Animation_Destroy = 0.25f;
const float Animation_Generate = 0.25f;
const float Animation_Fall = 0.25f;

Board::Board(int n, int m)
{
    current_ = nullptr;
    next_ = nullptr;
    tx_ = n;
    ty_ = m;
    layer_ = RenderManager::instance().AddLayer("board", 2);
    originX_ = 360;
    originY_ = 120;
    tileSize_ = 35;
    width_ = tx_ * tileSize_;
    height_ = ty_ * tileSize_;
    countScores = true;
    score = 0;
    scoreMultiplier = 1;
    state_ = BoardStates::Idle;
}

Board::~Board()
{
    Clear();
}

void Board::Fill()
{
    countScores = false;
    Clear();
    tiles_.resize(tx_);
    for (int i = 0; i < tx_; i++) {
        tiles_[i].resize(ty_);
        for (int j = 0; j < ty_; j++) {
            Tile& tile = tiles_[i][j];
            int item = rand() % spriteCount;
            tile.item = item;
            tile.sprite = new Sprite;
            tile.sprite->Init(spriteNames[item]);
            tile.i = i;
            tile.j = j;
            tile.removed = false;
            tile.sprite->position.x = (float)(originX_ + i * tileSize_);
            tile.sprite->position.y = (float)(originY_ + j * tileSize_);
            layer_->sprites.push_back(tile.sprite);
        }
    }
    // initial board state should be 'clear', without matches
    while (DestroyGems(false)) {
        GenerateGems(false);
    }
    AnimateInitial();
    countScores = true;
}

void Board::Clear()
{
    score = 0;
    layer_->sprites.clear();
    for (size_t i = 0; i < tiles_.size(); i++)
        for (size_t j = 0; j < tiles_[i].size(); j++)
            delete tiles_[i][j].sprite;
}

void Board::GenerateGems(bool animate)
{
    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < tx_; i++) {
        for (int j = 0; j < ty_; j++) {
            Tile& tile = tiles_[i][j];
            if (tile.removed) {
                tile.removed = false;
                int item = rand() % spriteCount;
                tile.item = item;
                Vec2 pos = tile.sprite->position;
                Color color = tile.sprite->color;
                tile.sprite->Init(spriteNames[item]);
                tile.sprite->position = pos;
                tile.sprite->color = color;
                if (animate) {
                    tile.sprite->color.a = 0.0f;
                    color.a = 1.0f;
                    animator.Add(new SpriteAnimateColor(Animation_Generate, tile.sprite, tile.sprite->color, color));
                }
                else {
                    tile.sprite->color.a = 1.0f;
                }
            }
        }
    }
}

bool Board::DestroyGems(bool animate)
{
    typedef std::vector<Tile*> Match;
    typedef std::vector<Match> Matches;
    Matches targets;
    // matches in rows
    for (int i = 0; i < tx_; i++) {
        int prev = -1;
        Match target;
        for (int j = 0; j < ty_; j++) {
            Tile& it = tiles_[i][j];
            if (it.item != prev)  {
                if (target.size() >= 3)
                    targets.push_back(target);
                target.clear();
            }
            target.push_back(&it);
            prev = it.item;
        }
        if (target.size() >= 3)
            targets.push_back(target);
    }
    // matches in columns
    for (int j = 0; j < ty_; j++) {
        int prev = -1;
        Match target;
        for (int i = 0; i < tx_; i++) {
            Tile& it = tiles_[i][j];
            if (it.item != prev)  {
                if (target.size() >= 3)
                    targets.push_back(target);
                target.clear();
            }
            target.push_back(&it);
            prev = it.item;
        }
        if (target.size() >= 3)
            targets.push_back(target);
    }
    // remove all matches from board
    AnimationManager& animator = AnimationManager::instance();
    for (size_t i = 0; i < targets.size(); i++) {
        Match& match = targets[i];
        int targetSize = (int)match.size();
        for (int j = 0; j < targetSize; j++) {
            Tile& tile = *match[j];
            tile.removed = true;
            if (animate) {
                Color c = tile.sprite->color;
                c.a = 0.0f;
                animator.Add(new SpriteAnimateColor(Animation_Destroy, tile.sprite, tile.sprite->color, c));
            }
            else {
                tile.sprite->color.a = 0.0f;
            }
        }
        if (countScores) {
            score += 10 * (targetSize - 2) * scoreMultiplier;
            scoreMultiplier++;
        }
    }

    return !targets.empty();
}

void Board::AnimateInitial()
{
    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < tx_; i++) {
        for (int j = 0; j < ty_; j++) {
            Tile& tile = tiles_[i][j];
            Sprite* sprite = tile.sprite;
            Vec2 endpos = sprite->position;
            Vec2 beginpos;
            beginpos.x = (float)(originX_ + i * tileSize_);
            beginpos.y = (float)(originY_ - 1 * tileSize_);
            animator.Add(new SpriteAnimatePosition(Animation_Appear, sprite, beginpos, endpos));
        }
    }
}

void Board::Update(float dt)
{
    if (!AnimationManager::instance().Empty())
        return;

    bool animate = true;
    if (state_ == BoardStates::Idle) {
        if (current_ != nullptr && next_ != nullptr) {
            Move(*current_, *next_, animate);
            state_ = BoardStates::Move;
        }
    }
    else if (state_ == BoardStates::Move) {
        if (EndMove(animate)) {
            state_ = BoardStates::Destroy;
        }
        else {
            CancelMove(animate);
            state_ = BoardStates::Idle;
        }
    }
    else if (state_ == BoardStates::Destroy) {
        FallGems(animate);
        state_ = BoardStates::Fall;
    }
    else if (state_ == BoardStates::Fall) {
        GenerateGems(animate);
        state_ = BoardStates::Generate;
    }
    else if (state_ == BoardStates::Generate) {
        if (DestroyGems(animate)) {
            state_ = BoardStates::Destroy;
        }
        else {
            scoreMultiplier = 1;
            state_ = BoardStates::Idle;
        }
    }
}

static bool IsPointInsideRect(int x, int y, int sx, int sy, int sw, int sh)
{
    int minx = sx;
    int miny = sy;
    int maxx = sx + sw;
    int maxy = sy + sh;
    return (x > minx && x < maxx) && (y > miny && y < maxy);
}

void Board::MouseDown(int x, int y)
{
    if (state_ != BoardStates::Idle)
        return;
    if (IsPointInsideRect(x, y, originX_, originY_, width_, height_)) {
        int i = (int)floor((x - originX_) / (float)tileSize_);
        int j = (int)floor((y - originY_) / (float)tileSize_);
        swipe_ = &tiles_[i][j];
    }
}

void Board::MouseUp(int x, int y)
{
    if (state_ != BoardStates::Idle)
        return;
    if (IsPointInsideRect(x, y, originX_, originY_, width_, height_)) {
        int i = (int)floor((x - originX_) / (float)tileSize_);
        int j = (int)floor((y - originY_) / (float)tileSize_);
        ClickOn(tiles_[i][j]);
    }
}

static bool IsNeighbor(const Tile& a, const Tile& b)
{
    if (a.i == b.i)
        return a.j == b.j + 1 || a.j == b.j - 1;
    if (a.j == b.j)
        return a.i == b.i + 1 || a.i == b.i - 1;
    return false;
}

void Board::ClickOn(Tile& tile)
{
    if (swipe_ != nullptr) {
        if (IsNeighbor(*swipe_, tile)) {
            current_ = swipe_;
            next_ = &tile;
            return;
        }
        swipe_ = nullptr;
    }

    if (current_ == nullptr)
        current_ = &tile;
    else if (IsNeighbor(*current_, tile))
        next_ = &tile;
    else
        current_ = &tile;
}

void Board::Move(Tile& a, Tile& b, bool animate)
{
    std::swap(tiles_[a.i][a.j], tiles_[b.i][b.j]);
    std::swap(a.i, b.i);
    std::swap(a.j, b.j);
    if (animate) {
        AnimationManager& animator = AnimationManager::instance();
        animator.Add(new SpriteAnimatePosition(Animation_Move, a.sprite, a.sprite->position, b.sprite->position));
        animator.Add(new SpriteAnimatePosition(Animation_Move, b.sprite, b.sprite->position, a.sprite->position));
    }
    else {
        std::swap(a.sprite->position, b.sprite->position);
    }
}

void Board::CancelMove(bool animate)
{
    Move(*current_, *next_, animate);
    current_ = nullptr;
    next_ = nullptr;
}

bool Board::EndMove(bool animate)
{
    if (DestroyGems(animate)) {
        current_ = nullptr;
        next_ = nullptr;
        return true;
    }
    return false;
}

void Board::FallGems(bool animate)
{
    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < tx_; i++) {
        while (true) {
            Tile* empty = FindEmpty(i);
            if (empty == nullptr)
                break;
            Tile* next = FindNotEmptyAbove(i, empty->j);
            if (next == nullptr)
                break;
            Move(*empty, *next, false);
            if (animate) {
                animator.Add(new SpriteAnimatePosition(Animation_Fall, empty->sprite, next->sprite->position, empty->sprite->position));
            }
        }
    }
}

Tile* Board::FindEmpty(int iCol)
{
    for (int j = ty_ - 1; j >= 0; j--) {
        Tile& tile = tiles_[iCol][j];
        if (tile.removed)
            return &tile;
    }
    return nullptr;
}

Tile* Board::FindNotEmptyAbove(int iCol, int jRow)
{
    for (int j = jRow; j >= 0; j--) {
        Tile& tile = tiles_[iCol][j];
        if (!tile.removed)
            return &tile;
    }
    return nullptr;
}

std::string Board::GetScoresText() const
{
    std::ostringstream ss;
    ss << GetScores();
    return ss.str();
}
