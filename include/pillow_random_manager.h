#ifndef PILLOW_RANDOM_MANAGER_H
#define PILLOW_RANDOM_MANAGER_H

#include <array>
#include <stdexcept>
#include "config.h"
#include "randomizer.h"
#include "screen.h"

class PillowRandomManager
{
   private:
    Randomizer<TerminalScreen::Coordinate> y_;
    Randomizer<TerminalScreen::SizeParam> width_;
    Randomizer<unsigned int> nonempty_up_height_ratio_;
    Randomizer<unsigned int> empty_height_ratio_;
    Randomizer<unsigned int> nonempty_down_height_ratio_;

    Randomizer<TerminalScreen::SizeParam> down_empty_height_;

    const TerminalScreen::SizeParam screen_start_y_, screen_height_;

   public:
    PillowRandomManager(
        const TerminalScreen &game_screen,
        const std::pair<unsigned int, unsigned int> &up_empty_range,
        const std::pair<unsigned int, unsigned int> &down_empty_range,
        const std::pair<unsigned int, unsigned int> &width_range,
        const std::pair<unsigned int, unsigned int> &nonempty_up_height_ratios_range,
        const std::pair<unsigned int, unsigned int> &empty_height_ratios_range,
        const std::pair<unsigned int, unsigned int> &nonempty_down_height_ratios_range)
        : y_(game_screen.start_y() + up_empty_range.first,
             game_screen.start_y() + up_empty_range.second),

          width_(width_range.first, width_range.second),

          nonempty_up_height_ratio_(nonempty_up_height_ratios_range.first,
                                    nonempty_up_height_ratios_range.second),

          empty_height_ratio_(empty_height_ratios_range.first, empty_height_ratios_range.second),

          nonempty_down_height_ratio_(nonempty_down_height_ratios_range.first,
                                      nonempty_down_height_ratios_range.second),
          down_empty_height_(down_empty_range.first, down_empty_range.second),
          screen_start_y_(game_screen.start_y()),
          screen_height_(game_screen.height())
    {
        if (screen_height_ < up_empty_range.second + down_empty_range.second + SCREEN_MIN_HEIGHT)
        {
            throw std::out_of_range(
                "Screeen height must be > PILLOW_UP_EMPTY_RANGE.second + "
                "PILLOW_DOWN_EMPTY_RANGE.second + MIN_HEIGHT.");
        }
    }

    TerminalScreen::Coordinate start_y();
    TerminalScreen::SizeParam width();

    std::array<TerminalScreen::SizeParam, 3> heights(const TerminalScreen::SizeParam y);
};

#endif  // !PILLOW_RANDOM_MANAGER_H
