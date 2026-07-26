/*
 * Screen based on ncurses.
 */
#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <cstdlib>
#include <utility>
#include "idrawable.h"
#include "iscreen2d.h"

using TerminalColor = short;

class TerminalScreen : public IScreen2D<TerminalColor>
{
   public:
    using SizeParam = typename IScreen2D<TerminalColor>::SizeParam;
    using DeltaVal = typename IScreen2D<TerminalColor>::DeltaVal;
    using Coordinate = typename IScreen2D<TerminalColor>::Coordinate;

   private:
    const SizeParam width_, height_;
    const DeltaVal delta_x_, delta_y_;
    const Coordinate start_val_x_, start_val_y_;
    const TerminalColor default_fgcolor_, default_bgcolor_;
    TerminalColor current_fgcolor_, current_bgcolor_;
    WINDOW *window_;

    std::pair<int, int> GetCursor() const noexcept;
    void SetCursorAbsolute(const SizeParam abs_x, const SizeParam abs_y);

   public:
    static TerminalScreen Init()
    {
        initscr();
        start_color();

        return TerminalScreen(stdscr, {0, 0}, {0, 0});
    }

    TerminalScreen(const std::pair<SizeParam, SizeParam> &size,
                   const std::pair<DeltaVal, DeltaVal> &delties,
                   const std::pair<Coordinate, Coordinate> &start_vals,
                   const std::pair<TerminalColor, TerminalColor> &colors = {COLOR_WHITE,
                                                                            COLOR_BLACK})
        : TerminalScreen(newwin(size.second, size.first, delties.second, delties.first),
                         delties,
                         start_vals,
                         colors)
    {
    }

    TerminalScreen(WINDOW *window,
                   const std::pair<DeltaVal, DeltaVal> &delties,
                   const std::pair<Coordinate, Coordinate> &start_vals,
                   const std::pair<TerminalColor, TerminalColor> &colors = {COLOR_WHITE,
                                                                            COLOR_BLACK})
        : width_(getmaxx(window)),
          height_(getmaxy(window)),
          delta_x_(delties.first),
          delta_y_(delties.second),
          start_val_x_(start_vals.first),
          start_val_y_(start_vals.second),
          default_fgcolor_(colors.first),
          default_bgcolor_(colors.second),
          current_fgcolor_(default_fgcolor_),
          current_bgcolor_(default_bgcolor_),
          window_(window)
    {
        init_pair(1, default_fgcolor_, default_bgcolor_);
        wbkgd(window_, COLOR_PAIR(1) | ' ');
        Update();
    }

    SizeParam width() const noexcept override { return width_; }
    SizeParam height() const noexcept override { return height_; }
    DeltaVal delta_x() const noexcept override { return delta_x_; }
    DeltaVal delta_y() const noexcept override { return delta_y_; }
    Coordinate start_val_x() const noexcept override { return start_val_x_; }
    Coordinate start_val_y() const noexcept override { return start_val_y_; }

    void Draw(const char symbol) override;
    void Update() override;

    void SetBackgroundColor(const TerminalColor &bg_color) override;
    void SetForegroundColor(const TerminalColor &fg_color) override;
    void FlushColor() override;

    TerminalScreen &operator<<(const TerminalColor &color);
    TerminalScreen &operator<<(const char symbol);
    TerminalScreen &operator<<(const IDrawable<TerminalColor> &shape);

    void SetCursor(const Coordinate x, const Coordinate y) override;
    void SetCursorStartVals() override;
    void MoveCursor(const Coordinate delta_x, const Coordinate delta_y) override;
    void SetCursorVisible(const bool visible = false) override;

    void Clear() override;

    ~TerminalScreen() override
    {
        if (window_ != stdscr)
        {
            delwin(window_);
        }
        else
        {
            endwin();
        }
    }
};

#endif  // !SCREEN_H
