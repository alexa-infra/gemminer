#pragma once

#include "base/types.h"

namespace base
{

//! Timer class, count time in seconds
class Timer
{
public:
    Timer();

    //! Reset timer to zero, returns current elapsed time in seconds
    f32 reset();

    //! Returns current elapsed time in milliseconds
    f32 elapsed() const;

private:
    //! Gets current timer value
    u64 getClock() const;

    //! Convert internal representation to milliseconds
    f64 convertToMillis( u64 range ) const;

private:
    u64 startTime_;
    u64 frequency_;
};

} // namespace base