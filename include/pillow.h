#ifndef PILLOW_H
#define PILLOW_H

#include <array>
#include <stdexcept>
#include "bird.h"
#include "icollision.h"
#include "idrawable.h"
#include "imovable.h"
#include "irectangle2d.h"
#include "iscreen2d.h"

template <typename Color>
class Pillow : public ICollision, public IDrawable<Color>, public IRectangle2D, public IMovable
{
   private:
    // Coordinates of left up vertex.
    double x_, y_;
    const SizeParam width_;

    const SizeParam non_empty_up_height_, empty_height_, non_empty_down_height_;

    const char start_symbol_, middle_symbol_, end_symbol_;
    const Color fg_color_;

    const Coordinate speed_x_per_second_;

    bool bird_passed_ = false;

   public:
    Coordinate start_x() const noexcept override { return x_; }
    Coordinate start_y() const noexcept override { return y_; }
    Coordinate end_x() const noexcept override { return x_ + static_cast<Coordinate>(width_) - 1; }
    Coordinate end_y() const noexcept override
    {
        return y_ + static_cast<Coordinate>(height()) - 1;
    }

    SizeParam width() const noexcept override { return width_; }
    SizeParam height() const noexcept override
    {
        return non_empty_up_height_ + empty_height_ + non_empty_down_height_;
    }

    bool IsPassed() const noexcept { return bird_passed_; }
    void SetPassed() noexcept { bird_passed_ = true; }

    Pillow(const std::pair<Coordinate, Coordinate> &start_coords,
           const SizeParam width,
           const std::pair<SizeParam, SizeParam> &non_empty_heights,
           const SizeParam empty_height,
           const std::array<char, 3> &symbols,
           const Color &color,
           const Coordinate speed_x_per_second)
        : x_(start_coords.first),
          y_(start_coords.second),
          width_(width),
          non_empty_up_height_(non_empty_heights.first),
          empty_height_(empty_height),
          non_empty_down_height_(non_empty_heights.second),
          start_symbol_(symbols.at(0)),
          middle_symbol_(symbols.at(1)),
          end_symbol_(symbols.at(2)),
          fg_color_(color),
          speed_x_per_second_(speed_x_per_second)
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
        if (bird.end_x() < start_x())
        {
            return false;
        }
        // >[
        if (start_x() == bird.end_x())
        {
            return (bird.start_y() >= start_y() &&
                    bird.start_y() < start_y() + non_empty_up_height_) ||
                   (bird.start_y() >= start_y() + non_empty_up_height_ + empty_height_ &&
                    bird.start_y() <
                        start_y() + non_empty_up_height_ + empty_height_ + non_empty_down_height_);
        }
        // [--]
        // <>
        // [--]
        if (bird.start_x() < end_x())
        {
            return (bird.start_y() >= start_y() &&
                    bird.start_y() < start_y() + non_empty_up_height_) ||
                   (bird.start_y() >= start_y() + non_empty_up_height_ + empty_height_ &&
                    bird.start_y() <
                        start_y() + non_empty_up_height_ + empty_height_ + non_empty_down_height_);
        }
        return false;
    }

    void Move(const double delta_secs_from_last_frame) override
    {
        x_ -= speed_x_per_second_ * delta_secs_from_last_frame;
    }

    void DrawOn(IScreen2D<Color> &screen) const override
    {
        const auto screen_end_x = screen.start_x() + screen.width() - 1;

        if (screen_end_x < x_)
        {
            return;
        }
        else if (x_ + width_ < screen.start_x())
        {
            return;
        }
        screen.SetForegroundColor(fg_color_);
        // up non empty.
        for (SizeParam i = 0; i < non_empty_up_height_; ++i)
        {
            if (x_ >= screen.start_x())
            {
                screen.SetCursor(x_, y_ + i);
                screen.Draw(start_symbol_);
            }
            else
            {
                screen.SetCursor(screen.start_x(), y_ + i);
            }

            SizeParam j = 1;
            while (j + 1 < width_ && x_ + j <= screen_end_x)
            {
                if (x_ + j >= screen.start_x())
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
        for (SizeParam i = 0; i < non_empty_down_height_; ++i)
        {
            if (x_ >= screen.start_x())
            {
                screen.SetCursor(x_, y_ + non_empty_up_height_ + empty_height_ + i);
                screen.Draw(start_symbol_);
            }
            else
            {
                screen.SetCursor(screen.start_x(), y_ + non_empty_up_height_ + empty_height_ + i);
            }

            SizeParam j = 1;
            while (j + 1 < width_ && x_ + j <= screen_end_x)
            {
                if (x_ + j >= screen.start_x())
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
