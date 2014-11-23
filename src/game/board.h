#pragma once

#include <vector>
#include <string>
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
        bool removed;
    };

    namespace BoardStates
    {
        enum BoardState
        {
            Idle = 0,
            Move,
            Destroy,
            MoveGems,
            Generate
        };
    }
    typedef BoardStates::BoardState BoardState;

    class Board
    {
    public:
        Board(int n, int m);
        ~Board();

        void clear();
        void fill();
        void click(int x, int y);
        void update(float dt);
        int getScores() const { return score; }
        std::string getScoresText() const;

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
        bool endMove(bool animate);

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
        int score;
        int scoreMultiplier;
        bool countScores;
        Tile* current_;
        Tile* next_;
        BoardState state_;
    };
}
