#include "screen.h"
#include "screen_rectangle.h"

int main()
{
    TerminalScreen::Init();

    TerminalScreen std_screen(stdscr, {0, 0}, {0, 0}, {COLOR_YELLOW + 8, COLOR_BLACK});
    TerminalScreen game_screen({5, 5}, {1, 1}, {0, 0}, {COLOR_BLUE + 8, COLOR_WHITE});
    ScreenRectangle<TerminalColor> game_rect(std_screen, game_screen, COLOR_YELLOW + 8);
    game_screen << game_rect;

    game_screen.Update();

    getch();

    TerminalScreen::Close();

    return 0;
}
