#include "game/board.h"
#include "game/render_manager.h"

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
    tx_ = n;
    ty_ = m;
    layer_ = RenderManager::instance().addLayer("board", 2);
    originX_ = 360;
    originY_ = 120;
    tileSize_ = 35;
    width_ = tx_ * tileSize_;
    height_ = ty_ * tileSize_;

    fill();
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
