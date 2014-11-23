#include "game/sprite.h"
#include "game/resource_manager.h"
#include "SDL.h"

using namespace game;

void Sprite::init(const std::string& path)
{
    color.r = 1.0f; color.g = 1.0f; color.b = 1.0f; color.a = 1.0f;
    position.x = 0.0f; position.y = 0.0f;
    ImageTexture img = ResourceManager::instance().Texture(path);
    tex = img.texture;
    w = img.w;
    h = img.h;
}

SpriteAnimatePosition::SpriteAnimatePosition(float d, Sprite* s, Vec2 from, Vec2 to)
{
    duration = d;
    clock = 0.0f;
    sprite = s;
    positionFrom = from;
    positionTo = to;
    setInitial();
}

void SpriteAnimatePosition::setInitial()
{
    sprite->position = positionFrom;
}

void SpriteAnimatePosition::setFinish()
{
    sprite->position = positionTo;
}

// t - time, b - begin, c - change (ending - beginning), d - duration
static float linear(float t, float b, float c, float d)
{
    return c * t / d + b;
}

void SpriteAnimatePosition::setEasing()
{
    Vec2 newPos;
    newPos.x = linear(clock, positionFrom.x, positionTo.x - positionFrom.x, duration);
    newPos.y = linear(clock, positionFrom.y, positionTo.y - positionFrom.y, duration);
    sprite->position = newPos;
}

SpriteAnimateColor::SpriteAnimateColor(float d, Sprite* s, Color from, Color to)
{
    duration = d;
    clock = 0.0f;
    sprite = s;
    colorFrom = from;
    colorTo = to;
    setInitial();
}

void SpriteAnimateColor::setInitial()
{
    sprite->color = colorFrom;
}

void SpriteAnimateColor::setFinish()
{
    sprite->color = colorTo;
}

void SpriteAnimateColor::setEasing()
{
    Color newColor;
    newColor.r = linear(clock, colorFrom.r, colorTo.r - colorFrom.r, duration);
    newColor.g = linear(clock, colorFrom.g, colorTo.g - colorFrom.g, duration);
    newColor.b = linear(clock, colorFrom.b, colorTo.b - colorFrom.b, duration);
    newColor.a = linear(clock, colorFrom.a, colorTo.a - colorFrom.a, duration);
    sprite->color = newColor;
}


