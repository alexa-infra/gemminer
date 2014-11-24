#include "game/sprite.h"
#include "game/resource_manager.h"
#include "SDL.h"

using namespace game;

void Sprite::Init(const std::string& path)
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    position.x = 0.0f;
    position.y = 0.0f;
    ImageTexture img = ResourceManager::instance().Texture(path);
    tex = img.texture;
    w = img.w;
    h = img.h;
}

SpriteAnimatePosition::SpriteAnimatePosition(float d, Sprite* s, Vec2 from, Vec2 to)
{
    duration_ = d;
    clock_ = 0.0f;
    sprite = s;
    positionFrom = from;
    positionTo = to;
    SetInitial();
}

void SpriteAnimatePosition::SetInitial()
{
    sprite->position = positionFrom;
}

void SpriteAnimatePosition::SetFinish()
{
    sprite->position = positionTo;
}

// t - time, b - begin, c - change (ending - beginning), d - duration
static float linear(float t, float b, float c, float d)
{
    return c * t / d + b;
}

void SpriteAnimatePosition::SetEasing()
{
    Vec2 newPos;
    newPos.x = linear(clock_, positionFrom.x, positionTo.x - positionFrom.x, duration_);
    newPos.y = linear(clock_, positionFrom.y, positionTo.y - positionFrom.y, duration_);
    sprite->position = newPos;
}

SpriteAnimateColor::SpriteAnimateColor(float d, Sprite* s, Color from, Color to)
{
    duration_ = d;
    clock_ = 0.0f;
    sprite = s;
    colorFrom = from;
    colorTo = to;
    SetInitial();
}

void SpriteAnimateColor::SetInitial()
{
    sprite->color = colorFrom;
}

void SpriteAnimateColor::SetFinish()
{
    sprite->color = colorTo;
}

void SpriteAnimateColor::SetEasing()
{
    Color newColor;
    newColor.r = linear(clock_, colorFrom.r, colorTo.r - colorFrom.r, duration_);
    newColor.g = linear(clock_, colorFrom.g, colorTo.g - colorFrom.g, duration_);
    newColor.b = linear(clock_, colorFrom.b, colorTo.b - colorFrom.b, duration_);
    newColor.a = linear(clock_, colorFrom.a, colorTo.a - colorFrom.a, duration_);
    sprite->color = newColor;
}
