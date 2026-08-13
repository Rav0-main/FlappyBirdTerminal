#ifndef BIRD_H
#define BIRD_H

#include <stdexcept>
#include "config.h"
#include "icollision.h"
#include "idrawable.h"
#include "imovable.h"
#include "irectangle2d.h"
#include "iscreen2d.h"

template <typename Color>
class Bird : public ICollision, public IDrawable<Color>, public IRectangle2D, public IMovable
{
   private:
    double x_, y_;
    Coordinate speed_y_per_second_;
    double secs_from_prev_upping_ = 0;
    const std::string picture_;
    const Color fg_color_;

   public:
    void set_start_x(const Coordinate new_x) noexcept { x_ = new_x; }
    void set_start_y(const Coordinate new_y) noexcept { y_ = new_y; }
    Coordinate start_x() const noexcept override { return x_; }
    Coordinate start_y() const noexcept override { return y_; }
    Coordinate end_x() const noexcept override { return x_ + static_cast<Coordinate>(width()) - 1; }
    Coordinate end_y() const noexcept override
    {
        return y_ + static_cast<Coordinate>(height()) - 1;
    }
    SizeParam width() const noexcept override { return picture_.length(); }
    SizeParam height() const noexcept override { return 1U; }

    Bird(const std::pair<Coordinate, Coordinate> &start_coords,
         const Coordinate speed_y_per_second,
         const std::string &picture,
         const Color &color)
        : x_(start_coords.first),
          y_(start_coords.second),
          speed_y_per_second_(speed_y_per_second),
          picture_(picture),
          fg_color_(color)
    {
        if (picture_.length() == 0)
        {
            throw std::out_of_range("Picture length must be > 0.");
        }
        if (picture_.find('\n') != picture_.npos)
        {
            throw std::invalid_argument("Picture must be height == 1 (don't include '\n')");
        }
    }

    void HandlePressedKey(const int key)
    {
        switch (key)
        {
            case 'w':
            case ' ':
                secs_from_prev_upping_ = 0.04;
                speed_y_per_second_ = -std::abs(speed_y_per_second_);
                y_ -= 0.8;
                break;
        }
    }

    void Reset() noexcept { secs_from_prev_upping_ = 0; }

    void Move(const double delta_secs_from_last_frame) override
    {
        /* dy = d[y0 + V0y * t + g * t^2]
         * dy = V0y * dt + g * t * dt
         */
        y_ += speed_y_per_second_ * delta_secs_from_last_frame +
              g * secs_from_prev_upping_ * delta_secs_from_last_frame;

        secs_from_prev_upping_ += delta_secs_from_last_frame;
    }

    bool HasCollisionWith(const ICollision &other) const override
    {
        return other.HasCollisionWith(*this);
    }

    void DrawOn(IScreen2D<Color> &screen) const override
    {
        screen.SetForegroundColor(fg_color_);
        screen.SetCursor(x_, y_);
        for (const char ch : picture_)
        {
            screen.Draw(ch);
        }
    }

    ~Bird() override = default;
};

#endif  // !COLLISION_H
