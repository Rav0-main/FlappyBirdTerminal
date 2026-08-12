#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
   private:
    using Time = decltype(std::chrono::high_resolution_clock::now());

    Time prev_time_;
    const unsigned int tick_time_;

   public:
    Timer(const unsigned int fps)
        : prev_time_(std::chrono::high_resolution_clock::now()), tick_time_(1e6 / fps)
    {
    }
    Timer() : Timer(1e6 + 1) {}

    double GetFrameTime();
    void Tick() const noexcept;
};

#endif  // !TIMER_H
