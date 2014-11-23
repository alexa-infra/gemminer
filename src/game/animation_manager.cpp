#include "game/animation_manager.h"

using namespace game;

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
