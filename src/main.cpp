#include "bird.h"
#include "config.h"
#include "screen.h"
#include "screen_rectangle.h"

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

    std_screen << game_screen_rectangle;

    Bird bird(2, 2, COLOR_RED);
    game_screen << bird;

    std_screen.Update();
    game_screen.Update();

    getch();

    return 0;
}
