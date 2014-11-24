#include "game/timer_display.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace game;

TimerDisplay::TimerDisplay(float seconds)
{
    current_ = seconds;
    run_ = true;
}

void TimerDisplay::Update(float dt)
{
    if (!run_)
        return;
    if (current_ > dt) {
        current_ -= dt;
    }
    else {
        current_ = 0.0f;
        run_ = false;
    }
}

std::string TimerDisplay::GetText() const
{
    std::ostringstream ss;
    int minutes = (int)floor(current_) / 60;
    int seconds = (int)floor(current_) % 60;
    ss << std::setfill('0') << std::setw(2) << minutes;
    ss << ':';
    ss << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

void TimerDisplay::Reset(float seconds)
{
    current_ = seconds;
    run_ = true;
}

bool TimerDisplay::Finished() const
{
    return !run_;
}
