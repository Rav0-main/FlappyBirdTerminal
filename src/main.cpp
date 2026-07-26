#include "screen.h"
#include "screen_rectangle.h"

int main()
{
    TerminalScreen std_screen = TerminalScreen::Init();

    TerminalScreen game_screen({5, 5}, {50, 3}, {0, 0}, {COLOR_RED + 8, COLOR_WHITE});
    ScreenRectangle<TerminalColor> game_rect(game_screen, COLOR_WHITE + 8);

    std_screen << game_rect;
    std_screen.Update();

    game_screen.SetCursorStartVals();
    game_screen.MoveCursor(2, 1);
    game_screen << '<' << '>';

    game_screen.Update();

    getch();

    return 0;
}
