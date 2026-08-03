/*
 * Screen based on ncurses.
 */
#ifndef SCREEN_H
#define SCREEN_H

#include <ncurses.h>
#include <unordered_map>
#include <utility>
#include "idrawable.h"
#include "iscreen2d.h"

using TerminalColor = int16_t;
/*
 * In ncurses.h color is short.
 */
static_assert(sizeof(TerminalColor) >= sizeof(short),
              "sizeof(short) must be <= 16 bits. On platform this project not supported.");

class TerminalScreen : public IScreen2D<TerminalColor>
{
   private:
    static inline std::unordered_map<uint32_t, decltype(COLOR_PAIRS)> color_pairs;
    const SizeParam width_, height_;
    const DeltaVal delta_x_, delta_y_;
    const Coordinate start_val_x_, start_val_y_;
    const TerminalColor default_fgcolor_, default_bgcolor_;
    TerminalColor current_fgcolor_, current_bgcolor_;
    WINDOW *window_;

    static decltype(COLOR_PAIRS) GetColorPairId(const TerminalColor &fg,
                                                const TerminalColor &bg) noexcept;
    void SetCursorAbsolute(const SizeParam abs_x, const SizeParam abs_y);

   public:
    static TerminalScreen Init()
    {
        initscr();
        start_color();

        return TerminalScreen(stdscr, {0, 0}, {0, 0});
    }
    static void GameModeOn()
    {
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        curs_set(0);
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
        const auto color_pair_id = GetColorPairId(default_fgcolor_, default_bgcolor_);
        init_pair(color_pair_id, default_fgcolor_, default_bgcolor_);
        wattron(window_, COLOR_PAIR(color_pair_id) | ' ');
        Update();
    }

    Coordinate end_val_x() const noexcept override
    {
        return start_val_x_ + static_cast<Coordinate>(width_) - 1;
    }
    Coordinate end_val_y() const noexcept override
    {
        return start_val_y_ + static_cast<Coordinate>(height_) - 1;
    }
    SizeParam width() const noexcept override { return width_; }
    SizeParam height() const noexcept override { return height_; }
    DeltaVal delta_x() const noexcept override { return delta_x_; }
    DeltaVal delta_y() const noexcept override { return delta_y_; }
    Coordinate start_val_x() const noexcept override { return start_val_x_; }
    Coordinate start_val_y() const noexcept override { return start_val_y_; }
    TerminalColor default_fgcolor() const noexcept { return default_fgcolor_; }
    TerminalColor default_bgcolor() const noexcept { return default_bgcolor_; }

    void Draw(const char symbol) override;
    void Update() override;

    void SetBackgroundColor(const TerminalColor &bg_color) override;
    void SetForegroundColor(const TerminalColor &fg_color) override;
    void FlushColor() override;

    TerminalScreen &operator<<(const TerminalColor &color);
    TerminalScreen &operator<<(const char symbol);
    TerminalScreen &operator<<(const IDrawable<TerminalColor> &shape);
    TerminalScreen &operator<<(const char *msg);

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
