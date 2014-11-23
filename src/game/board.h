#pragma once

#include <vector>
#include "base/types.h"

namespace game
{
    struct RenderLayer;
    struct Sprite;
    struct Tile
    {
        Sprite* sprite;
        int item;
        int i;
        int j;
        int score;
        bool removed;
    };

    class Board
    {
    public:
        Board(int n, int m);
        ~Board();

        void clear();
        void fill();
        void click(float x, float y);
        void update(float dt);

    private:
        void clickOn(Tile& tile);

        Tile* findEmpty(int iCol);
        Tile* findNotEmptyAbove(int iCol, int jRow);

        void animateInitial();
        void moveGems(bool animate);
        void generateGems(bool animate);
        bool destroyGems(bool animate);
        void move(Tile& a, Tile& b, bool animate);
        void cancelMove(bool animate);
        void endMove(bool animate);

    private:
        typedef std::vector<std::vector<Tile>> Tiles;

        Tiles tiles_;
        RenderLayer* layer_;
        int tileSize_;
        int width_;
        int height_;
        int tx_;
        int ty_;
        int originX_;
        int originY_;
    };
}
