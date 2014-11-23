#pragma once

namespace game
{
    struct BaseAnimation
    {
        float clock;
        float duration;

        bool update(float dt);
        bool set(float newClock);
        virtual void setInitial() = 0;
        virtual void setFinish() = 0;
        virtual void setEasing() = 0;
    };
}