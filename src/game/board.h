#pragma once

#include <vector>
#include <string>
#include "base/types.h"

namespace game
{
struct RenderLayer;
struct Sprite;

//! Board tile, or single gem object
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
    Fall,
    Generate
};
}
typedef BoardStates::BoardState BoardState;

//! Gameplay class
//!   Provides logic of NxN gem board, destruction, and creation gems
class Board
{
public:
    Board(int n, int m);
    ~Board();

    //! Clears board, resets all states
    void Clear();
    //! Fill board with new set of gems
    void Fill();
    //! Click started
    void MouseDown(int x, int y);
    //! Click ended
    void MouseUp(int x, int y);
    //! Update state machine
    void Update(float dt);
    //! Get scores as text
    std::string GetScoresText() const;
    int GetScores() const {
        return score;
    }

private:
    //! Click at specific gem
    void ClickOn(Tile& tile);
    //! Find first empty tile in column looking from bottom to up
    Tile* FindEmpty(int iCol);
    //! Find first not-empty tile in column looking from bottom to up
    Tile* FindNotEmptyAbove(int iCol, int jRow);
    //! Animate initial appearing of gems
    void AnimateInitial();
    //! After destroying gems, already existing falls bottom on empty cells
    void FallGems(bool animate);
    //! Generate new gems at empty places
    void GenerateGems(bool animate);
    //! Look for matches in row/column, and if any then destroy them and return true, otherwise false
    bool DestroyGems(bool animate);
    //! Swap two neighbor gems
    void Move(Tile& a, Tile& b, bool animate);
    //! Cancel swapping (move gems back
    void CancelMove(bool animate);
    //! Check if swapping gems has any result, if yes, then destroy gems and return true
    bool EndMove(bool animate);

private:
    typedef std::vector<std::vector<Tile>> Tiles;

    Tiles tiles_;           //!< Gems 2d array
    RenderLayer* layer_;    //!< Layer for drawing gems
    int tileSize_;          //!< Size of single sprite in pixels
    int width_;             //!< Board width in pixels
    int height_;            //!< Board height in pixels
    int tx_;                //!< Number of tiles in row
    int ty_;                //!< Number of tiles in column
    int originX_;           //!< Origin coordinate of board
    int originY_;           //!< Origin coordinate of board
    int score;              //!< Current user score
    int scoreMultiplier;    //!< Score multiplier (increases for combos)
    bool countScores;       //!< Flag for disable counting scores
    Tile* swipe_;           //!< Temporary tile for swipe move
    Tile* current_;         //!< Temporary tile for single click move
    Tile* next_;            //!< Temporary tile for single click move
    BoardState state_;      //!< Current board state
};
}
