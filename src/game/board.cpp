#include "game/board.h"
#include "game/render_manager.h"
#include <cmath>

using namespace game;
using namespace base;

const int spriteCount = 5;
std::string spriteNames[spriteCount] = {
    "data/Blue.png",
    "data/Green.png",
    "data/Purple.png",
    "data/Red.png",
    "data/Yellow.png"
};

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

    fill();
    state_ = BoardStates::Idle;
}

Board::~Board()
{
    clear();
}

void Board::fill()
{
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
}

void Board::clear()
{
    layer_->sprites.clear();
    for (int i = 0; i < tiles_.size(); i++)
        for (int j = 0; j < tiles_[i].size(); j++)
            delete tiles_[i][j].sprite;
}

void Board::generateGems(bool animate)
{
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

    for (int i = 0; i < targets.size(); i++)
    {
        Match& match = targets[i];
        int targetSize = (int)match.size();
        for (int j = 0; j < targetSize; j++)
        {
            Tile& tile = *match[j];
            tile.removed = true;
            tile.score += targetSize;

            if (animate) {

            }
            else {
                tile.sprite->color.a = 0.0f;
            }
        }
    }

    return !targets.empty();
}

void Board::animateInitial()
{
}

void Board::update(float dt)
{
    bool animate = false;
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

void Board::click(int x, int y)
{
    if (state_ != BoardStates::Idle)
        return;
    if (isPointInsideRect(x, y, originX_, originY_, width_, height_)) {
        int i = (int)floor((x - originX_) / tileSize_);
        int j = (int)floor((y - originY_) / tileSize_);
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
            move(*empty, *next, animate);
            if (animate) {

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
