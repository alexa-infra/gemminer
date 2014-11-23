#pragma once

#include "base/singleton.h"
#include <vector>

namespace game
{
    struct BaseAnimation
    {
        virtual ~BaseAnimation(){}
        float clock;
        float duration;

        bool update(float dt);
        bool set(float newClock);
        virtual void setInitial() = 0;
        virtual void setFinish() = 0;
        virtual void setEasing() = 0;
    };

    class AnimationManager : public ::base::Singleton<AnimationManager>
    {
    public:
        AnimationManager();
        ~AnimationManager();

        void add(BaseAnimation* animation);
        bool empty() const;
        void update(float dt);
    private:
        std::vector<BaseAnimation*> animations_;
    };
}