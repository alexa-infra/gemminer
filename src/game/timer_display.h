#pragma once

#include <string>

namespace game
{
class TimerDisplay
{
public:
    explicit TimerDisplay(float seconds);
    void update(float dt);
    std::string getText() const;
    void reset(float seconds);
    bool isFinished() const;
private:
    float current_;
    bool run_;
};
}
