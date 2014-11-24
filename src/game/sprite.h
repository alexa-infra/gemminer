#pragma once

#include "base/types.h"
#include <string>
#include "game/animation_manager.h"

struct SDL_Texture;

namespace game
{
//! Position
struct Vec2
{
    float x;
    float y;
};

//! Color with floating point [0.0f; 1.0f]
struct Color
{
    float r, g, b, a;
};

//! Sprite
struct Sprite
{
    void Init(const std::string& path);

    SDL_Texture* tex;
    Color color;
    Vec2 position;
    int w;
    int h;
};

class SpriteAnimatePosition : public BaseAnimation
{
public:
    SpriteAnimatePosition(float duration, Sprite* s, Vec2 from, Vec2 to);

protected:
    void SetInitial();
    void SetFinish();
    void SetEasing();

public:
    Sprite* sprite;
    Vec2 positionFrom;
    Vec2 positionTo;
};

struct SpriteAnimateColor : public BaseAnimation
{
public:
    SpriteAnimateColor(float duration, Sprite* s, Color from, Color to);

protected:
    void SetInitial();
    void SetFinish();
    void SetEasing();

public:
    Sprite* sprite;
    Color colorFrom;
    Color colorTo;
};
}
