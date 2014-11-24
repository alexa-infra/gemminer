#pragma once

#include "base/singleton.h"
#include <vector>

namespace game
{
//! Base class for animations
class BaseAnimation
{
public:
    virtual ~BaseAnimation() {}
    
    bool Update(float dt);
    bool Set(float newClock);

protected:
    virtual void SetInitial() = 0;
    virtual void SetFinish() = 0;
    virtual void SetEasing() = 0;
protected:
    float clock_;
    float duration_;
};

//! Animation manager, updates all animations, and remove them on finish
class AnimationManager : public ::base::Singleton<AnimationManager>
{
public:
    AnimationManager();
    ~AnimationManager();

    void Add(BaseAnimation* animation);
    bool Empty() const;
    void Update(float dt);
private:
    std::vector<BaseAnimation*> animations_;
};
}