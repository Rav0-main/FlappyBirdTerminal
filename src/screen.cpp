#include "screen.h"
#include <ncurses.h>

void TerminalScreen::Draw(const char symbol)
{
    waddch(window_, symbol);
}

void TerminalScreen::Update()
{
    wrefresh(window_);
}

void TerminalScreen::SetBackgroundColor(const TerminalColor &bg_color)
{
    wattroff(window_, COLOR_PAIR(1));
    init_pair(1, current_fgcolor_, bg_color);
    wattron(window_, COLOR_PAIR(1));

    current_bgcolor_ = std::move(bg_color);
}

void TerminalScreen::SetForegroundColor(const TerminalColor &fg_color)
{
    wattroff(window_, COLOR_PAIR(1));
    init_pair(1, fg_color, current_bgcolor_);
    wattron(window_, COLOR_PAIR(1));

    current_fgcolor_ = std::move(fg_color);
}

void TerminalScreen::FlushColor()
{
    wattroff(window_, COLOR_PAIR(1));
    init_pair(1, default_fgcolor_, default_bgcolor_);
    wattron(window_, COLOR_PAIR(1));

    current_fgcolor_ = default_fgcolor_;
    current_bgcolor_ = default_bgcolor_;
}

TerminalScreen &TerminalScreen::operator<<(const char symbol)
{
    Draw(symbol);
    return *this;
}

TerminalScreen &TerminalScreen::operator<<(const TerminalColor &fg_color)
{
    SetForegroundColor(fg_color);
    return *this;
}

TerminalScreen &TerminalScreen::operator<<(const IDrawable<TerminalColor> &shape)
{
    shape.DrawOn(*this);
    return *this;
}

void TerminalScreen::SetCursorAbsolute(const SizeParam abs_x, const SizeParam abs_y)
{
    wmove(window_, abs_y, abs_x);
}

void TerminalScreen::SetCursor(const Coordinate x, const Coordinate y)
{
    Coordinate new_x = (x - start_val_x_) % width_;
    new_x = new_x < 0 ? new_x + width_ : new_x;

    Coordinate new_y = (y - start_val_y_) % height_;
    new_y = new_y < 0 ? new_y + height_ : new_y;

    SetCursorAbsolute(new_x, new_y);
}

void TerminalScreen::SetCursorStartVals()
{
    SetCursorAbsolute(0, 0);
}

void TerminalScreen::SetCursorVisible(const bool visible)
{
    curs_set(visible);
}

void TerminalScreen::MoveCursor(const Coordinate delta_x, const Coordinate delta_y)
{
    Coordinate current_x, current_y;
    getyx(window_, current_y, current_x);

    SetCursor(current_x + delta_x, current_y + delta_y);
}

void TerminalScreen::Clear()
{
    werase(window_);
}
