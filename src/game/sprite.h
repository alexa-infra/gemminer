#pragma once

#include "base/types.h"
#include <string>
#include "game/animation_manager.h"

struct SDL_Texture;

namespace game
{
    struct Vec2
    {
        float x;
        float y;
    };

    struct Color
    {
        float r, g, b, a;
    };

    struct Sprite
    {
        SDL_Texture* tex;
        Color color;
        Vec2 position;
        int w;
        int h;

        void init(const std::string& path);
    };

    struct SpriteAnimatePosition : public BaseAnimation
    {
        Sprite* sprite;
        Vec2 positionFrom;
        Vec2 positionTo;

        SpriteAnimatePosition(float duration, Sprite* s, Vec2 from, Vec2 to);
        
        void setInitial();
        void setFinish();
        void setEasing();
    };

    struct SpriteAnimateColor : public BaseAnimation
    {
        Sprite* sprite;
        Color colorFrom;
        Color colorTo;

        SpriteAnimateColor(float duration, Sprite* s, Color from, Color to);

        void setInitial();
        void setFinish();
        void setEasing();
    };
}
