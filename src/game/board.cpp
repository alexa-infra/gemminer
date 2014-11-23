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
}

void Board::clear()
{
    layer_->sprites.clear();
    for (int i = 0; i < tiles_.size(); i++)
        for (int j = 0; j < tiles_[i].size(); j++)
            delete tiles_[i][j].sprite;
}
