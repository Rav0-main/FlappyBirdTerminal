#include "screen.h"
#include <stdexcept>

void TerminalScreen::Draw(const char symbol)
{
    waddch(window_, symbol);
}

void TerminalScreen::Draw(const IDrawable &shape)
{
    shape.Draw();
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

TerminalScreen &TerminalScreen::operator<<(const IDrawable &shape)
{
    Draw(shape);
    return *this;
}

void TerminalScreen::SetCursorAbsolute(const SizeParam abs_x, const SizeParam abs_y)
{
    wmove(window_, abs_y, abs_x);
}

void TerminalScreen::SetCursor(const Coordinate x, const Coordinate y)
{
    if (x < start_val_x_)
    {
        throw std::out_of_range("X value must be >= " + std::to_string(start_val_x_));
    }
    if (y < start_val_y_)
    {
        throw std::out_of_range("Y value must be >= " + std::to_string(start_val_y_));
    }
    SetCursorAbsolute((x - start_val_x_) % width_, (y - start_val_y_) % height_);
}

void TerminalScreen::SetCursorStartVals() noexcept
{
    SetCursorAbsolute(0, 0);
}

void TerminalScreen::SetCursorVisible(const bool visible) noexcept
{
    curs_set(visible);
}

void TerminalScreen::Clear() noexcept
{
    werase(window_);
}
