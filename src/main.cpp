#include <unistd.h>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <format>
#include <fstream>
#include "bird.h"
#include "config.h"
#include "pillow.h"
#include "pillow_random_manager.h"
#include "screen.h"
#include "screen_rectangle.h"
#include "text.h"
#include "timer.h"

static inline uint32_t GetBestScore()
{
    if (!std::filesystem::exists(BEST_SCORE_FILENAME))
    {
        return 0;
    }
    std::ifstream fin(BEST_SCORE_FILENAME, std::ios::binary);
    if (!fin.is_open())
    {
        return 0;
    }

    uint32_t best_score = 0;
    fin.read(reinterpret_cast<char *>(&best_score), sizeof(best_score));

    return best_score;
}

static inline TerminalScreen GetGameScreenIn(const TerminalScreen &std_screen)
{
    TerminalScreen::SizeParam width = std_screen.width() * RATIO_SCREEN_WIDTH;
    TerminalScreen::SizeParam height = std_screen.height() * RATIO_SCREEN_HEIGHT;

    if (height > SCREEN_CRITICAL_HEIGHT)
    {
        return TerminalScreen(
            {width, height}, {(std_screen.width() - width) / 2, (std_screen.height() - height) / 2},
            {0, 0}, {std_screen.current_fgcolor(), std_screen.current_bgcolor()});
    }

    return TerminalScreen({width, std_screen.height() - 2}, {(std_screen.width() - width) / 2, 1},
                          {0, 0}, {std_screen.current_fgcolor(), std_screen.current_bgcolor()});
}

static inline void DrawScreenRectangleFor(const TerminalScreen &screen,
                                          TerminalScreen &where,
                                          const TerminalColor rectangle_color)
{
    ScreenRectangle<TerminalColor> screen_rectangle(screen, rectangle_color);
    where << screen_rectangle;
}

static inline void GeneratePillows(std::deque<Pillow<TerminalColor>> &pillows,
                                   const TerminalScreen &game_screen,
                                   PillowRandomManager &pillow_randomizer)
{
    const auto x_prev = 3 * game_screen.width() / 4;
    TerminalScreen::SizeParam sum_width_prev = 0;

    for (unsigned int i = 0; i < game_screen.width() / RATIO_WIDTH_PER_PILLOW; ++i)
    {
        const auto y = pillow_randomizer.start_y();
        const auto [nonempty_up_height, empty_height, nonempty_down_height] =
            pillow_randomizer.heights(y);

        pillows.emplace_back(
            std::make_pair(x_prev + i * RATIO_WIDTH_PER_PILLOW + sum_width_prev, y),
            pillow_randomizer.width(), std::make_pair(nonempty_up_height, nonempty_down_height),
            empty_height, PILLOW_PICTURE, COLOR_GREEN, PILLOW_SPEED_X_PER_SECOND);

        sum_width_prev += pillows.back().width();
    }
}

static inline void FPSTick(const unsigned short fps)
{
    usleep(1e6L / fps);
}

static inline void WriteBestScore(uint32_t best_score)
{
    std::ofstream fout(BEST_SCORE_FILENAME, std::ios::binary);
    if (fout.is_open())
    {
        fout.write(reinterpret_cast<char *>(&best_score), sizeof(best_score));
    }
}

int main()
{
    TerminalScreen std_screen = TerminalScreen::Init();
    TerminalScreen game_screen = GetGameScreenIn(std_screen);

    uint32_t best_score = GetBestScore();

    if (game_screen.height() < SCREEN_MIN_HEIGHT)
    {
        std_screen << "You have very small screen height :(";
        std_screen.GetKey();
        return 1;
    }
    if (game_screen.width() < RATIO_WIDTH_PER_PILLOW)
    {
        std_screen << "You have very small screen width :(";
        std_screen.GetKey();
        return 1;
    }

    Bird<TerminalColor> bird(
        {game_screen.width() * RATIO_BIRD_POSITION_X, game_screen.height() * RATIO_BIRD_POSITION_Y},
        BIRD_SPEED_0Y, BIRD_PICTURE, COLOR_RED);

    PillowRandomManager pillow_randomizer(
        game_screen, PILLOW_UP_EMPTY_RANGE, PILLOW_DOWN_EMPTY_RANGE, PILLOW_WIDTH_RANGE,
        PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE, PILLOW_EMPTY_HEIGHT_RATIOS_RANGE,
        PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE);

    // Do cycle buffer (for remove memory allocations on every push/pop pillow).
    std::deque<Pillow<TerminalColor>> pillows;
    GeneratePillows(pillows, game_screen, pillow_randomizer);
    TerminalScreen::GameModeOn();

    std_screen.Clear();
    DrawScreenRectangleFor(game_screen, std_screen, COLOR_WHITE + 8);
    std_screen.Update();

    bool run = true;
    bool paused = false;
    uint32_t score = 0;
    Timer clock;
    while (run)
    {
        int key = game_screen.GetKey();

        // Handle pressed key by player
        if (key != ERR)
        {
            if (!paused && bird.IsAlive())
            {
                bird.HandlePressedKey(key);
            }

            switch (key)
            {
                case 'w':
                    if (paused && bird.IsAlive())
                    {
                        bird.set_start_y(bird.start_y() - 1);
                    }
                    break;

                case 's':
                    if (paused && bird.IsAlive())
                    {
                        bird.set_start_y(bird.start_y() + 1);
                    }
                    break;

                case 'a':
                    if (paused && bird.IsAlive())
                    {
                        bird.set_start_x(bird.start_x() - 1);
                    }
                    break;

                case 'd':
                    if (paused && bird.IsAlive())
                    {
                        bird.set_start_x(bird.start_x() + 1);
                    }
                    break;

                case 'p':
                    paused = !paused || !bird.IsAlive();
                    break;

                case 'r':
                    paused = false;
                    best_score = std::max(best_score, score);
                    pillows.clear();
                    GeneratePillows(pillows, game_screen, pillow_randomizer);
                    bird.Revive();
                    bird.set_start_x(game_screen.width() * RATIO_BIRD_POSITION_X);
                    bird.set_start_y(game_screen.height() * RATIO_BIRD_POSITION_Y);
                    score = 0;
                    break;

                case 'q':
                case 'Q':
                    run = false;
                    break;
            }
        }

        // Spawn new pillow
        if (game_screen.end_x() - pillows.back().end_x() >=
            static_cast<TerminalScreen::Coordinate>(RATIO_WIDTH_PER_PILLOW))
        {
            const auto y = pillow_randomizer.start_y();
            const auto [nonempty_up_height, empty_height, nonempty_down_height] =
                pillow_randomizer.heights(y);

            pillows.emplace_back(
                std::make_pair(game_screen.start_x() + game_screen.width(), y),
                pillow_randomizer.width(), std::make_pair(nonempty_up_height, nonempty_down_height),
                empty_height, PILLOW_PICTURE, COLOR_GREEN, PILLOW_SPEED_X_PER_SECOND);
        }

        const auto frame_time = clock.GetFrameTime();
        if (bird.IsAlive() && !paused)
        {
            bird.Move(frame_time);
        }

        // Move pillows
        if (!paused)
        {
            for (auto &p : pillows)
            {
                p.Move(frame_time);
                if (p.end_x() < game_screen.start_x())
                {
                    pillows.pop_front();
                }
            }
        }

        // Bird collision with pillows
        if (bird.IsAlive() && !paused)
        {
            for (const auto &p : pillows)
            {
                if (p.HasCollisionWith(bird))
                {
                    bird.Kill();
                    best_score = std::max(best_score, score);
                    paused = true;
                }
            }
        }

        // Bird collision with up and down screen borders
        if (!paused && bird.IsAlive() &&
            (bird.start_y() == game_screen.start_y() ||
             bird.start_y() == game_screen.start_y() + game_screen.height() - 1))
        {
            bird.Kill();
            paused = true;
        }

        // Calculate score
        if (bird.IsAlive() && !paused)
        {
            for (auto &p : pillows)
            {
                if (p.end_x() + 1 == bird.end_x() && !p.IsPassed())
                {
                    ++score;
                    p.SetPassed();
                    break;
                }
            }
        }

        // Drawing frame
        game_screen.Clear();
        for (const auto &p : pillows)
        {
            game_screen << p;
        }

        Text<TerminalColor> text_status;
        if (!paused && bird.IsAlive())
        {
            text_status = Text<TerminalColor>(std::format("Score: {}\n", score).c_str(),
                                              {COLOR_WHITE, game_screen.current_bgcolor()},
                                              {game_screen.start_x(), game_screen.start_y()});
        }
        else if (!bird.IsAlive())
        {
            game_screen << Text<TerminalColor>(std::format("Game Over!\n", score).c_str(),
                                               {COLOR_RED, game_screen.current_bgcolor()},
                                               {game_screen.start_x(), game_screen.start_y()});

            text_status = Text<TerminalColor>(std::format("Score: {}\n", score).c_str(),
                                              {COLOR_WHITE, game_screen.current_bgcolor()});
        }
        else
        {
            text_status =
                Text<TerminalColor>("Game Paused.\n", {COLOR_YELLOW, game_screen.current_bgcolor()},
                                    {game_screen.start_x(), game_screen.start_y()});
        }

        game_screen << text_status;

        Text<TerminalColor> text_best_score(std::format("Best score: {}", best_score).c_str(),
                                            {COLOR_CYAN, game_screen.current_bgcolor()});

        game_screen << text_best_score;

        game_screen << bird;
        game_screen.Update();

        FPSTick(FPS);
    }

    best_score = std::max(best_score, score);
    WriteBestScore(best_score);

    return 0;
}
