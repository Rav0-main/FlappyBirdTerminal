#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
   private:
    using Time = decltype(std::chrono::high_resolution_clock::now());
    Time prev_time_;

   public:
    Timer() : prev_time_(std::chrono::high_resolution_clock::now()) {}

    double GetFrameTime();
};

#endif  // !TIMER_H
