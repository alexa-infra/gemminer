#pragma once

#include <string>

namespace game
{

//! Countdown timer
class TimerDisplay
{
public:
    explicit TimerDisplay(float seconds);
    void Update(float dt);
    std::string GetText() const;
    void Reset(float seconds);
    bool Finished() const;
private:
    float current_;
    bool run_;
};

} // namespace game
