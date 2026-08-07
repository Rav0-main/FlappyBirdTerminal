#include "timer.h"

double Timer::GetFrameTime()
{
    Time current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = current_time - prev_time_;
    prev_time_ = current_time;

    return elapsed.count();
}
