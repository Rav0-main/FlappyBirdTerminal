#include "pillow_random_manager.h"
#include <algorithm>
#include "screen.h"

TerminalScreen::Coordinate PillowRandomManager::start_y()
{
    return y_();
}

TerminalScreen::SizeParam PillowRandomManager::width()
{
    return width_();
}

std::array<TerminalScreen::SizeParam, 3> PillowRandomManager::heights(
    const TerminalScreen::SizeParam y)
{
    const auto H = screen_height_ - y - screen_start_y_ - down_empty_height_() + 1;
    const auto Hd = static_cast<double>(H);

    const auto i = nonempty_up_height_ratio_();
    const auto j = empty_height_ratio_();
    const auto k = nonempty_down_height_ratio_();

    std::array<double, 3> exact = {Hd * i / (i + j + k), Hd * j / (i + j + k),
                                   Hd * k / (i + j + k)};

    std::array<TerminalScreen::SizeParam, 3> heights;
    std::array<double, 3> remainders;

    for (unsigned short i = 0; i < 3; ++i)
    {
        heights[i] = static_cast<TerminalScreen::SizeParam>(exact[i]);
        remainders[i] = exact[i] - heights[i];
    }

    TerminalScreen::SizeParam current_sum = std::accumulate(heights.begin(), heights.end(), 0U);
    TerminalScreen::SizeParam p = H - current_sum;

    std::array<unsigned short, 3> indexes = {0, 1, 2};
    std::sort(indexes.begin(), indexes.end(), [&remainders](unsigned short i, unsigned short j)
              { return remainders[i] > remainders[j]; });

    for (unsigned short i = 0; i < p; ++i)
    {
        ++heights[indexes[i]];
    }

    for (unsigned short i = 0; i < 3; ++i)
    {
        if (heights[i] < 1)
        {
            heights[i] = 1;
            auto max_iter = std::max_element(heights.begin(), heights.end());
            --(*max_iter);
        }
    }

    return heights;
}
