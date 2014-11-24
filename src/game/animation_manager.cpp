#include "game/animation_manager.h"

using namespace game;

template<> AnimationManager* base::Singleton<AnimationManager>::instance_ = nullptr;

bool BaseAnimation::Update(float dt)
{
    return Set(clock_ + dt);
}

bool BaseAnimation::Set(float newClock)
{
    clock_ = newClock;
    if (clock_ < 0) {
        clock_ = 0;
        SetInitial();
    }
    else if (clock_ >= duration_) {
        clock_ = duration_;
        SetFinish();
    }
    else {
        SetEasing();
    }
    return clock_ >= duration_;
}

AnimationManager::AnimationManager()
{

}

AnimationManager::~AnimationManager()
{
    for (size_t i = 0; i < animations_.size(); i++)
        delete animations_[i];
}

void AnimationManager::Add(BaseAnimation* animation)
{
    animations_.push_back(animation);
}

bool AnimationManager::Empty() const
{
    return animations_.empty();
}

void AnimationManager::Update(float dt)
{
    size_t i = 0;
    while (i < animations_.size())
    {
        BaseAnimation* animation = animations_[i];
        if (animation->Update(dt))
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
