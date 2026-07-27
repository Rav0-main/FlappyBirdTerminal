#include <unistd.h>
#include "bird.h"
#include "config.h"
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

    Bird bird(game_screen.width() * RATIO_BIRD_POSITION_X,
              game_screen.height() * RATIO_BIRD_POSITION_Y, COLOR_RED);

    TerminalScreen::GameModeOn();

    bool run = true;
    while (run)
    {
        char key = getch();
        if (key != ERR)
        {
            switch (key)
            {
                case 'w':
                    bird.set_y(bird.y() - 1);
                    break;

                case 's':
                    bird.set_y(bird.y() + 1);
                    break;

                case 'd':
                    bird.set_x(bird.x() + 1);
                    break;

                case 'a':
                    bird.set_x(bird.x() - 1);
                    break;

                case 'q':
                case 'Q':
                    run = false;
            }
        }

        std_screen.Clear();
        std_screen << game_screen_rectangle;
        std_screen.Update();

        game_screen << bird;
        game_screen.Update();

        fps_tick(FPS);
    }

    return 0;
}
