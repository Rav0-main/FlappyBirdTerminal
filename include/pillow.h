#ifndef PILLOW_H
#define PILLOW_H

#include <stdexcept>
#include <tuple>
#include "bird.h"
#include "icollision.h"
#include "idrawable.h"
#include "iscreen2d.h"

template <typename Color>
class Pillow : public ICollision, public IDrawable<Color>
{
   private:
    // Coordinates of left up vertex.
    IScreen2D<Color>::Coordinate x_, y_;
    const IScreen2D<Color>::SizeParam width_;

    const IScreen2D<Color>::SizeParam non_empty_up_height_, empty_height_, non_empty_down_height_;

    const char start_symbol_, middle_symbol_, end_symbol_;
    const Color fg_color_;

   public:
    typename IScreen2D<Color>::Coordinate x() const noexcept { return x_; }
    typename IScreen2D<Color>::Coordinate y() const noexcept { return y_; }

    void set_x(const typename IScreen2D<Color>::Coordinate new_x) noexcept { x_ = new_x; }

    typename IScreen2D<Color>::SizeParam width() const noexcept { return width_; }

    Pillow(const std::pair<typename IScreen2D<Color>::Coordinate,
                           typename IScreen2D<Color>::Coordinate> &start_coords,
           const typename IScreen2D<Color>::SizeParam width,
           const std::pair<typename IScreen2D<Color>::SizeParam,
                           typename IScreen2D<Color>::SizeParam> &non_empty_heights,
           const typename IScreen2D<Color>::SizeParam empty_height,
           const std::tuple<char, char, char> &symbols,
           const Color &color)
        : x_(start_coords.first),
          y_(start_coords.second),
          width_(width),
          non_empty_up_height_(non_empty_heights.first),
          empty_height_(empty_height),
          non_empty_down_height_(non_empty_heights.second),
          start_symbol_(std::get<0>(symbols)),
          middle_symbol_(std::get<1>(symbols)),
          end_symbol_(std::get<2>(symbols)),
          fg_color_(color)
    {
        if (width_ < 2)
        {
            throw std::out_of_range("Pillow width must be >= 2.");
        }
    }
    bool HasCollisionWith(const ICollision &other) const override
    {
        return other.HasCollisionWith(*this);
    }
    bool HasCollisionWith(const Bird<Color> &bird) const
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
                   bird.y() ==
                       y_ + non_empty_up_height_ + empty_height_ + non_empty_down_height_ - 1;
        }
        return false;
    }

    void DrawOn(IScreen2D<Color> &screen) const override
    {
        const auto screen_end_x = screen.start_val_x() + screen.width() - 1;

        if (screen_end_x < x_)
        {
            return;
        }
        else if (x_ + width_ < screen.start_val_x())
        {
            return;
        }
        screen.SetForegroundColor(fg_color_);
        // up non empty.
        for (typename IScreen2D<Color>::SizeParam i = 0; i < non_empty_up_height_; ++i)
        {
            if (x_ >= screen.start_val_x())
            {
                screen.SetCursor(x_, y_ + i);
                screen.Draw(start_symbol_);
            }
            else
            {
                screen.SetCursor(screen.start_val_x(), y_ + i);
            }

            typename IScreen2D<Color>::SizeParam j = 1;
            while (j + 1 < width_ && x_ + j <= screen_end_x)
            {
                if (x_ + j >= screen.start_val_x())
                {
                    screen.Draw(middle_symbol_);
                }
                ++j;
            }

            if (x_ + width_ <= screen_end_x)
            {
                screen.Draw(end_symbol_);
            }
        }

        // down non empty.
        for (typename IScreen2D<Color>::SizeParam i = 0; i < non_empty_down_height_; ++i)
        {
            if (x_ >= screen.start_val_x())
            {
                screen.SetCursor(x_, y_ + non_empty_up_height_ + empty_height_ + i);
                screen.Draw(start_symbol_);
            }
            else
            {
                screen.SetCursor(screen.start_val_x(),
                                 y_ + non_empty_up_height_ + empty_height_ + i);
            }

            typename IScreen2D<Color>::SizeParam j = 1;
            while (j + 1 < width_ && x_ + j <= screen_end_x)
            {
                if (x_ + j >= screen.start_val_x())
                {
                    screen.Draw(middle_symbol_);
                }
                ++j;
            }
            if (x_ + width_ <= screen_end_x)
            {
                screen.Draw(end_symbol_);
            }
        }
    }

    ~Pillow() override = default;
};

#endif  // !PILLOW_H
