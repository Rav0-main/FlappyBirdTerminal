#include <unistd.h>
#include <algorithm>
#include <array>
#include <deque>
#include <stdexcept>
#include "bird.h"
#include "config.h"
#include "pillow.h"
#include "randomizer.h"
#include "screen.h"
#include "screen_rectangle.h"

class PillowRandomManager
{
   private:
    Randomizer<TerminalScreen::Coordinate> y_;
    Randomizer<TerminalScreen::SizeParam> width_;
    Randomizer<unsigned int> nonempty_up_height_ratio_;
    Randomizer<unsigned int> empty_height_ratio_;
    Randomizer<unsigned int> nonempty_down_height_ratio_;

    Randomizer<TerminalScreen::SizeParam> down_empty_height_{PILLOW_DOWN_EMPTY_RANGE.first,
                                                             PILLOW_DOWN_EMPTY_RANGE.second};

    const TerminalScreen::SizeParam screen_start_val_y_, screen_height_;

   public:
    PillowRandomManager(const TerminalScreen &game_screen)
        : y_(game_screen.start_val_y() + PILLOW_UP_EMPTY_RANGE.first,
             game_screen.start_val_y() + PILLOW_UP_EMPTY_RANGE.second),

          width_(PILLOW_WIDTH_RANGE.first, PILLOW_WIDTH_RANGE.second),

          nonempty_up_height_ratio_(PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE.first,
                                    PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE.second),

          empty_height_ratio_(PILLOW_EMPTY_HEIGHT_RATIOS_RANGE.first,
                              PILLOW_EMPTY_HEIGHT_RATIOS_RANGE.second),

          nonempty_down_height_ratio_(PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE.first,
                                      PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE.second),
          screen_start_val_y_(game_screen.start_val_y()),
          screen_height_(game_screen.height())
    {
        if (screen_height_ <
            PILLOW_UP_EMPTY_RANGE.second + PILLOW_DOWN_EMPTY_RANGE.second + SCREEN_MIN_HEIGHT)
        {
            throw std::out_of_range(
                "Screeen height must be > PILLOW_UP_EMPTY_RANGE.second + "
                "PILLOW_DOWN_EMPTY_RANGE.second + HEIGHT_MIN.");
        }
    }

    TerminalScreen::Coordinate y() { return y_(); }
    TerminalScreen::SizeParam width() { return width_(); }

    std::array<TerminalScreen::SizeParam, 3> heights(const TerminalScreen::SizeParam y)
    {
        const auto H = screen_height_ - y - screen_start_val_y_ - down_empty_height_() + 1;
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
};

static inline TerminalScreen GetGameScreen(const TerminalScreen &std_screen)
{
    TerminalScreen::SizeParam width = std_screen.width() * RATIO_SCREEN_WIDTH;
    TerminalScreen::SizeParam height = std_screen.height() * RATIO_SCREEN_HEIGHT;

    if (height > SCREEN_CRITICAL_HEIGHT)
    {
        return TerminalScreen(
            {width, height}, {(std_screen.width() - width) / 2, (std_screen.height() - height) / 2},
            {0, 0}, {std_screen.default_fgcolor(), std_screen.default_bgcolor()});
    }

    return TerminalScreen({width, std_screen.height() - 2}, {(std_screen.width() - width) / 2, 1},
                          {0, 0}, {std_screen.default_fgcolor(), std_screen.default_bgcolor()});
}

static inline void GeneratePillows(std::deque<Pillow<TerminalColor>> &pillows,
                                   const TerminalScreen &game_screen,
                                   PillowRandomManager &pillow_randomizer)
{
    const auto x_prev = game_screen.width();
    TerminalScreen::SizeParam sum_width_prev = 0;
    for (unsigned int i = 0; i < game_screen.width() / RATIO_WIDTH_PER_PILLOW; ++i)
    {
        const auto y = pillow_randomizer.y();
        const auto [nonempty_up_height, empty_height, nonempty_down_height] =
            pillow_randomizer.heights(y);

        pillows.emplace_back(
            std::make_pair(x_prev + i * RATIO_WIDTH_PER_PILLOW + sum_width_prev, y),
            pillow_randomizer.width(), std::make_pair(nonempty_up_height, nonempty_down_height),
            empty_height,
            std::make_tuple(PILLOW_START_PICTURE, PILLOW_MIDDLE_PICTURE, PILLOW_END_PICTURE),
            COLOR_GREEN);

        sum_width_prev += pillows.back().width();
    }
}

static inline void FPSTick(const unsigned short fps)
{
    usleep(1e6L / fps);
}

int main()
{
    TerminalScreen std_screen = TerminalScreen::Init();
    TerminalScreen game_screen = GetGameScreen(std_screen);

    if (game_screen.height() < SCREEN_MIN_HEIGHT)
    {
        std_screen << "You have very small screen height :(";
        getch();
        return 1;
    }
    if (game_screen.width() < RATIO_WIDTH_PER_PILLOW)
    {
        std_screen << "You have very small screen width :(";
        getch();
        return 1;
    }

    ScreenRectangle<TerminalColor> game_screen_rectangle(game_screen, COLOR_WHITE + 8);

    Bird<TerminalColor> bird(
        {game_screen.width() * RATIO_BIRD_POSITION_X, game_screen.height() * RATIO_BIRD_POSITION_Y},
        BIRD_PICTURE, COLOR_RED);

    PillowRandomManager pillow_randomizer(game_screen);
    /*
     * Do cycle buffer (for remove memory allocations on every push/pop pillow).
     */
    std::deque<Pillow<TerminalColor>> pillows;
    GeneratePillows(pillows, game_screen, pillow_randomizer);
    TerminalScreen::GameModeOn();

    std_screen.Clear();
    std_screen << game_screen_rectangle;
    std_screen.Update();

    bool run = true;
    bool pillows_stopped = false;
    bool bird_dead = false;

    while (run)
    {
        int key = getch();
        if (key != ERR)
        {
            switch (key)
            {
                case 'w':
                    if (!bird_dead)
                        bird.set_y(bird.y() - 1);
                    break;

                case 's':
                    if (!bird_dead)
                        bird.set_y(bird.y() + 1);
                    break;

                case 'd':
                    if (!bird_dead)
                        bird.set_x(bird.x() + 1);
                    break;

                case 'a':
                    if (!bird_dead)
                        bird.set_x(bird.x() - 1);
                    break;

                case ' ':
                    pillows_stopped = !pillows_stopped || bird_dead;
                    break;

                case 'q':
                case 'Q':
                    run = false;
                    break;
            }
        }

        if (game_screen.start_val_x() + game_screen.width() - pillows.back().x() -
                pillows.back().width() >=
            static_cast<TerminalScreen::Coordinate>(RATIO_WIDTH_PER_PILLOW))
        {
            const auto y = pillow_randomizer.y();
            const auto [nonempty_up_height, empty_height, nonempty_down_height] =
                pillow_randomizer.heights(y);

            pillows.emplace_back(
                std::make_pair(game_screen.start_val_x() + game_screen.width(), y),
                pillow_randomizer.width(), std::make_pair(nonempty_up_height, nonempty_down_height),
                empty_height,
                std::make_tuple(PILLOW_START_PICTURE, PILLOW_MIDDLE_PICTURE, PILLOW_END_PICTURE),
                COLOR_GREEN);
        }

        for (const auto &p : pillows)
        {
            if (p.HasCollisionWith(bird))
            {
                bird_dead = true;
                pillows_stopped = true;
            }
        }

        if (!bird_dead)
        {
            bird_dead = bird.y() == game_screen.start_val_y() ||
                        bird.y() == game_screen.start_val_y() + game_screen.height() - 1;
            pillows_stopped = bird_dead;
        }

        if (!pillows_stopped)
        {
            for (auto &p : pillows)
            {
                p.set_x(p.x() - 1);
                if (p.x() + p.width() < game_screen.start_val_x())
                {
                    pillows.pop_front();
                }
            }
        }

        game_screen.Clear();
        for (const auto &p : pillows)
        {
            game_screen << p;
        }
        game_screen << bird;
        game_screen.Update();

        FPSTick(FPS);
    }

    return 0;
}
