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
    layer_ = RenderManager::instance().addLayer("board", 2);
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
    clear();
}

void Board::fill()
{
    countScores = false;
    clear();
    tiles_.resize(tx_);
    for (int i = 0; i < tx_; i++)
    {
        tiles_[i].resize(ty_);
        for (int j = 0; j < ty_; j++)
        {
            Tile& tile = tiles_[i][j];
            int item = rand() % spriteCount;
            tile.item = item;
            tile.sprite = new Sprite;
            tile.sprite->init(spriteNames[item]);
            tile.i = i;
            tile.j = j;
            tile.removed = false;
            tile.sprite->position.x = (float)(originX_ + i * tileSize_);
            tile.sprite->position.y = (float)(originY_ + j * tileSize_);
            layer_->sprites.push_back(tile.sprite);
        }
    }
    while (destroyGems(false)) {
        generateGems(false);
    }
    animateInitial();
    countScores = true;
}

void Board::clear()
{
    score = 0;
    layer_->sprites.clear();
    for (int i = 0; i < tiles_.size(); i++)
        for (int j = 0; j < tiles_[i].size(); j++)
            delete tiles_[i][j].sprite;
}

void Board::generateGems(bool animate)
{
    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < tx_; i++)
    {
        for (int j = 0; j < ty_; j++)
        {
            Tile& tile = tiles_[i][j];
            if (tile.removed) {
                tile.removed = false;
                int item = rand() % spriteCount;
                tile.item = item;
                Vec2 pos = tile.sprite->position;
                Color color = tile.sprite->color;
                tile.sprite->init(spriteNames[item]);
                tile.sprite->position = pos;
                tile.sprite->color = color;
                if (animate) {
                    tile.sprite->color.a = 0.0f;
                    color.a = 1.0f;
                    animator.add(new SpriteAnimateColor(Animation_Generate, tile.sprite, tile.sprite->color, color));
                }
                else {
                    tile.sprite->color.a = 1.0f;
                }
            }
        }
    }
}

bool Board::destroyGems(bool animate)
{
    typedef std::vector<Tile*> Match;
    typedef std::vector<Match> Matches;
    Matches targets;
    for (int i = 0; i < tx_; i++)
    {
        int prev = -1;
        Match target;
        for (int j = 0; j < ty_; j++)
        {
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

    for (int j = 0; j < ty_; j++)
    {
        int prev = -1;
        Match target;
        for (int i = 0; i < tx_; i++)
        {
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

    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < targets.size(); i++)
    {
        Match& match = targets[i];
        int targetSize = (int)match.size();
        for (int j = 0; j < targetSize; j++)
        {
            Tile& tile = *match[j];
            tile.removed = true;

            if (animate) {
                Color c = tile.sprite->color;
                c.a = 0.0f;
                animator.add(new SpriteAnimateColor(Animation_Destroy, tile.sprite, tile.sprite->color, c));
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

void Board::animateInitial()
{
    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < tx_; i++)
    {
        for (int j = 0; j < ty_; j++)
        {
            Tile& tile = tiles_[i][j];
            Sprite* sprite = tile.sprite;
            Vec2 endpos = sprite->position;
            Vec2 beginpos;
            beginpos.x = (float)(originX_ + i * tileSize_);
            beginpos.y = (float)(originY_ - 1 * tileSize_);
            animator.add(new SpriteAnimatePosition(Animation_Appear, sprite, beginpos, endpos));
        }
    }
}

void Board::update(float dt)
{
    if (!AnimationManager::instance().empty())
        return;

    bool animate = true;
    if (state_ == BoardStates::Idle) {
        if (current_ != nullptr && next_ != nullptr) {
            move(*current_, *next_, animate);
            state_ = BoardStates::Move;
        }
    }
    else if (state_ == BoardStates::Move) {
        if (endMove(animate)) {
            state_ = BoardStates::Destroy;
        }
        else {
            cancelMove(animate);
            state_ = BoardStates::Idle;
        }
    }
    else if (state_ == BoardStates::Destroy) {
        moveGems(animate);
        state_ = BoardStates::MoveGems;
    }
    else if (state_ == BoardStates::MoveGems) {
        generateGems(animate);
        state_ = BoardStates::Generate;
    }
    else if (state_ == BoardStates::Generate) {
        if (destroyGems(animate)) {
            state_ = BoardStates::Destroy;
        }
        else {
            scoreMultiplier = 1;
            state_ = BoardStates::Idle;
        }
    }
}

static bool isPointInsideRect(int x, int y, int sx, int sy, int sw, int sh)
{
    int minx = sx;
    int miny = sy;
    int maxx = sx + sw;
    int maxy = sy + sh;
    return (x > minx && x < maxx) && (y > miny && y < maxy);
}

void Board::mouseDown(int x, int y)
{
    if (state_ != BoardStates::Idle)
        return;
    if (isPointInsideRect(x, y, originX_, originY_, width_, height_)) {
        int i = (int)floor((x - originX_) / (float)tileSize_);
        int j = (int)floor((y - originY_) / (float)tileSize_);
        swipe_ = &tiles_[i][j];
    }
}

void Board::mouseUp(int x, int y)
{
    if (state_ != BoardStates::Idle)
        return;
    if (isPointInsideRect(x, y, originX_, originY_, width_, height_)) {
        int i = (int)floor((x - originX_) / (float)tileSize_);
        int j = (int)floor((y - originY_) / (float)tileSize_);
        clickOn(tiles_[i][j]);
    }
}

static bool isNeighbor(const Tile& a, const Tile& b)
{
    if (a.i == b.i)
        return a.j == b.j + 1 || a.j == b.j - 1;
    if (a.j == b.j)
        return a.i == b.i + 1 || a.i == b.i - 1;
    return false;
}

void Board::clickOn(Tile& tile)
{
    if (swipe_ != nullptr) {
        if (isNeighbor(*swipe_, tile)) {
            current_ = swipe_;
            next_ = &tile;
            return;
        }
        swipe_ = nullptr;
    }

    if (current_ == nullptr)
        current_ = &tile;
    else if (isNeighbor(*current_, tile))
        next_ = &tile;
    else
        current_ = &tile;
}

void Board::move(Tile& a, Tile& b, bool animate)
{
    int i = a.i;
    int j = a.j;
    int i1 = b.i;
    int j1 = b.j;
    std::swap(tiles_[i][j], tiles_[i1][j1]);
    std::swap(a.i, b.i);
    std::swap(a.j, b.j);
    if (animate) {
        AnimationManager& animator = AnimationManager::instance();
        animator.add(new SpriteAnimatePosition(Animation_Move, a.sprite, a.sprite->position, b.sprite->position));
        animator.add(new SpriteAnimatePosition(Animation_Move, b.sprite, b.sprite->position, a.sprite->position));
    }
    else {
        std::swap(a.sprite->position, b.sprite->position);
    }
}

void Board::cancelMove(bool animate)
{
    move(*current_, *next_, animate);
    current_ = nullptr;
    next_ = nullptr;
}

bool Board::endMove(bool animate)
{
    if (destroyGems(animate)) {
        current_ = nullptr;
        next_ = nullptr;
        return true;
    }
    return false;
}

void Board::moveGems(bool animate)
{
    AnimationManager& animator = AnimationManager::instance();
    for (int i = 0; i < tx_; i++)
    {
        while (true)
        {
            Tile* empty = findEmpty(i);
            if (empty == nullptr)
                break;
            Tile* next = findNotEmptyAbove(i, empty->j);
            if (next == nullptr)
                break;
            move(*empty, *next, false);
            if (animate) {
                animator.add(new SpriteAnimatePosition(Animation_Fall, empty->sprite, next->sprite->position, empty->sprite->position));
            }
        }
    }
}

Tile* Board::findEmpty(int iCol)
{
    for (int j = ty_ - 1; j >= 0; j--)
    {
        Tile& tile = tiles_[iCol][j];
        if (tile.removed)
            return &tile;
    }
    return nullptr;
}

Tile* Board::findNotEmptyAbove(int iCol, int jRow)
{
    for (int j = jRow; j >= 0; j--)
    {
        Tile& tile = tiles_[iCol][j];
        if (!tile.removed)
            return &tile;
    }
    return nullptr;
}

std::string Board::getScoresText() const
{
    std::ostringstream ss;
    ss << getScores();
    return ss.str();
}
