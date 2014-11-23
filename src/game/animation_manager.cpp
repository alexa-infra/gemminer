#include "game/animation_manager.h"

using namespace game;

AnimationManager* base::Singleton<AnimationManager>::instance_ = nullptr;

bool BaseAnimation::update(float dt)
{
    return set(clock + dt);
}

bool BaseAnimation::set(float newClock)
{
    clock = newClock;
    if (clock < 0) {
        clock = 0;
        setInitial();
    }
    else if (clock >= duration) {
        clock = duration;
        setFinish();
    }
    else {
        setEasing();
    }
    return clock >= duration;
}

AnimationManager::AnimationManager()
{

}

AnimationManager::~AnimationManager()
{
    for (size_t i = 0; i < animations_.size(); i++)
        delete animations_[i];
}

void AnimationManager::add(BaseAnimation* animation)
{
    animations_.push_back(animation);
}

bool AnimationManager::empty() const
{
    return animations_.empty();
}

void AnimationManager::update(float dt)
{
    int i = 0;
    while (i < animations_.size())
    {
        BaseAnimation* animation = animations_[i];
        if (animation->update(dt))
        {
            animations_.erase(animations_.begin() + i);
            delete animation;
        }
        else
        {
            i++;
        }
    }
}
