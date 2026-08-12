#include "timer.h"
#include <thread>

double Timer::GetFrameTime()
{
    Time current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = current_time - prev_time_;
    prev_time_ = current_time;

    return elapsed.count();
}

void Timer::Tick() const noexcept
{
    std::this_thread::sleep_for(std::chrono::microseconds(tick_time_));
}
