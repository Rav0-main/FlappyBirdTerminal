#include <deque>
#include <format>
#include "best_score_manager.h"
#include "bird.h"
#include "config.h"
#include "pillow.h"
#include "pillow_random_manager.h"
#include "screen.h"
#include "screen_rectangle.h"
#include "text.h"
#include "timer.h"

using GameStatus = enum { RUNNING, PAUSE, END };

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

int main()
{
    TerminalScreen std_screen = TerminalScreen::Init();
    TerminalScreen game_screen = GetGameScreenIn(std_screen);

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

    BestScoreManager best_score(BEST_SCORE_FILENAME);

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
    GameStatus game_status = RUNNING;
    BestScoreManager::Score score = 0;
    Timer clock(FPS);
    while (run)
    {
        int key = game_screen.GetKey();

        // Handle pressed key by player
        if (key != ERR)
        {
            if (game_status == RUNNING)
            {
                bird.HandlePressedKey(key);
            }

            switch (key)
            {
                case 'w':
                    if (game_status == PAUSE)
                    {
                        bird.set_start_y(bird.start_y() - 1);
                    }
                    break;

                case 's':
                    if (game_status == PAUSE)
                    {
                        bird.set_start_y(bird.start_y() + 1);
                    }
                    break;

                case 'a':
                    if (game_status == PAUSE)
                    {
                        bird.set_start_x(bird.start_x() - 1);
                    }
                    break;

                case 'd':
                    if (game_status == PAUSE)
                    {
                        bird.set_start_x(bird.start_x() + 1);
                    }
                    break;

                case 'p':
                    if (game_status == PAUSE || game_status == RUNNING)
                    {
                        game_status = game_status == PAUSE ? RUNNING : PAUSE;
                    }
                    break;

                case 'r':
                    game_status = RUNNING;

                    best_score = std::max(best_score.current(), score);
                    score = 0;

                    pillows.clear();
                    GeneratePillows(pillows, game_screen, pillow_randomizer);

                    bird.set_start_x(game_screen.width() * RATIO_BIRD_POSITION_X);
                    bird.set_start_y(game_screen.height() * RATIO_BIRD_POSITION_Y);
                    break;

                case 'q':
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
        if (game_status == RUNNING)
        {
            bird.Move(frame_time);
        }

        // Move pillows
        if (game_status == RUNNING)
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
        if (game_status == RUNNING)
        {
            for (const auto &p : pillows)
            {
                if (p.HasCollisionWith(bird))
                {
                    best_score = std::max(best_score.current(), score);
                    game_status = END;
                }
            }
        }

        // Bird collision with up and down screen borders
        if (game_status == RUNNING &&
            (bird.start_y() == game_screen.start_y() ||
             bird.start_y() == game_screen.start_y() + game_screen.height() - 1))
        {
            game_status = END;
        }

        // Calculate score
        if (game_status == RUNNING)
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
        if (game_status == RUNNING)
        {
            text_status = Text<TerminalColor>(std::format("Score: {}\n", score).c_str(),
                                              {COLOR_WHITE, game_screen.current_bgcolor()},
                                              {game_screen.start_x(), game_screen.start_y()});
        }
        else if (game_status == END)
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

        Text<TerminalColor> text_best_score(
            std::format("Best score: {}", best_score.current()).c_str(),
            {COLOR_CYAN, game_screen.current_bgcolor()});

        game_screen << text_best_score;

        game_screen << bird;
        game_screen.Update();

        clock.Tick();
    }

    best_score = std::max(best_score.current(), score);
    best_score.Write();

    return 0;
}
