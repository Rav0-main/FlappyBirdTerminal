#include <unistd.h>
#include "bird.h"
#include "config.h"
#include "pillow.h"
#include "screen.h"
#include "screen_rectangle.h"

static inline void fps_tick(const unsigned short fps)
{
    usleep(1e6L / fps);
}

int main()
{
    TerminalScreen std_screen = TerminalScreen::Init();

    TerminalScreen::SizeParam game_screen_width = std_screen.width() * RATIO_WIDTH;
    TerminalScreen::SizeParam game_screen_height = std_screen.height() * RATIO_HEIGHT;

    TerminalScreen game_screen({game_screen_width, game_screen_height},
                               {(std_screen.width() - game_screen_width) / 2,
                                (std_screen.height() - game_screen_height) / 2},
                               {0, 0},
                               {std_screen.default_fgcolor(), std_screen.default_bgcolor()});
    ScreenRectangle<TerminalColor> game_screen_rectangle(game_screen, COLOR_WHITE + 8);

    Bird<TerminalColor> bird(
        {game_screen.width() * RATIO_BIRD_POSITION_X, game_screen.height() * RATIO_BIRD_POSITION_Y},
        BIRD_PICTURE, COLOR_RED);

    TerminalScreen::GameModeOn();
    Pillow<TerminalColor> pillow(
        {game_screen.start_val_x() + game_screen.width() - 1, 1}, 5, {2, 1}, 1,
        {PILLOW_START_PICTURE, PILLOW_MIDDLE_PICTURE, PILLOW_END_PICTURE}, COLOR_GREEN);

    std_screen.Clear();
    std_screen << game_screen_rectangle;
    std_screen.Update();

    bool run = true;
    bool dead = false;
    while (run)
    {
        int key = getch();
        if (key != ERR)
        {
            switch (key)
            {
                case 'w':
                    if (!dead)
                        bird.set_y(bird.y() - 1);
                    break;

                case 's':
                    if (!dead)
                        bird.set_y(bird.y() + 1);
                    break;

                case 'd':
                    if (!dead)
                        bird.set_x(bird.x() + 1);
                    break;

                case 'a':
                    if (!dead)
                        bird.set_x(bird.x() - 1);
                    break;

                case 'q':
                case 'Q':
                    run = false;
            }
        }

        pillow.set_x(pillow.x() - 1);
        if (pillow.x() + pillow.width() < game_screen.start_val_x())
        {
            pillow.set_x(game_screen.start_val_x() + game_screen.width() + 1);
        }

        if (pillow.HasCollisionWith(bird))
        {
            dead = true;
        }
        if (!dead)
        {
            game_screen.Clear();
            game_screen << pillow << bird;
            game_screen.Update();
        }

        fps_tick(FPS);
    }

    return dead;
}
