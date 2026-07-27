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
                               {0, 0}, {COLOR_RED + 8, COLOR_WHITE});
    ScreenRectangle<TerminalColor> game_screen_rectangle(game_screen, COLOR_WHITE + 8);

    std_screen << game_screen_rectangle;
    std_screen.Update();

    game_screen.SetCursorStartVals();
    game_screen.MoveCursor(1, 1);
    game_screen << '<' << '>';

    game_screen.Update();

    getch();

    return 0;
}
