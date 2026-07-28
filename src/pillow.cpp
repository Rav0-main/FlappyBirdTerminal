#include "pillow.h"

bool Pillow::HasCollisionWith(const ICollision &other) const
{
    return other.HasCollisionWith(*this);
}

bool Pillow::HasCollisionWith(const Bird &bird) const
{
    const auto bird_end_x = bird.x() + static_cast<decltype(bird.x())>(bird.length()) - 1;
    if (bird_end_x < x_)
    {
        return false;
    }
    if (x_ == bird_end_x)
    {
        return (bird.y() >= y_ && bird.y() < y_ + non_empty_up_height_) ||
               (bird.y() >= y_ + non_empty_up_height_ + empty_height_ &&
                bird.y() < y_ + non_empty_up_height_ + empty_height_ + non_empty_down_height_);
    }
    if (bird.x() >= x_ && bird_end_x <= x_ + width_)
    {
        return bird.y() == y_ || bird.y() == y_ + non_empty_up_height_ - 1 ||
               bird.y() == y_ + non_empty_up_height_ + empty_height_ ||
               bird.y() == y_ + non_empty_up_height_ + empty_height_ + non_empty_down_height_ - 1;
    }
    return false;
}

void Pillow::DrawOn(IScreen2D<TerminalColor> &screen) const
{
    const auto screen_end_x = screen.start_val_x() + screen.width();

    if (screen_end_x < x_)
    {
        return;
    }
    screen.SetForegroundColor(fg_color_);
    // up non empty.
    for (IScreen2D<TerminalColor>::SizeParam i = 0; i < non_empty_up_height_; ++i)
    {
        screen.SetCursor(x_, y_ + i);
        screen.Draw(start_symbol_);

        IScreen2D<TerminalColor>::SizeParam j = 1;
        while (j + 1 < width_ && x_ + j <= screen_end_x)
        {
            screen.Draw(middle_symbol_);
            ++j;
        }

        if (x_ + width_ <= screen_end_x)
        {
            screen.Draw(end_symbol_);
        }
    }

    // down non empty.
    for (IScreen2D<TerminalColor>::SizeParam i = 0; i < non_empty_down_height_; ++i)
    {
        screen.SetCursor(x_, y_ + non_empty_up_height_ + empty_height_ + i);
        screen.Draw(start_symbol_);

        IScreen2D<TerminalColor>::SizeParam j = 1;
        while (j + 1 < width_ && x_ + j <= screen_end_x)
        {
            screen.Draw(middle_symbol_);
            ++j;
        }
        if (x_ + width_ <= screen_end_x)
        {
            screen.Draw(end_symbol_);
        }
    }
}
