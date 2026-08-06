#ifndef BIRD_H
#define BIRD_H

#include <stdexcept>
#include "icollision.h"
#include "idrawable.h"
#include "imovable.h"
#include "irectangle2d.h"
#include "iscreen2d.h"

template <typename Color>
class Bird : public ICollision, public IDrawable<Color>, public IRectangle2D, public IMovable
{
   private:
    Coordinate x_, y_;
    bool is_dead_;
    const std::string picture_;
    const Color fg_color_;

   public:
    Coordinate start_x() const noexcept override { return x_; }
    Coordinate start_y() const noexcept override { return y_; }
    Coordinate end_x() const noexcept override { return x_ + static_cast<Coordinate>(width()) - 1; }
    Coordinate end_y() const noexcept override
    {
        return y_ + static_cast<Coordinate>(height()) - 1;
    }
    SizeParam width() const noexcept override { return picture_.length(); }
    SizeParam height() const noexcept override { return 1U; }

    bool IsAlive() const noexcept { return !is_dead_; }

    // :(
    void Kill() noexcept { is_dead_ = true; }

    Bird(const std::pair<typename IScreen2D<Color>::Coordinate,
                         typename IScreen2D<Color>::Coordinate> &start_coords,
         const std::string &picture,
         const Color &color)
        : x_(start_coords.first),
          y_(start_coords.second),
          is_dead_(false),
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
                if (!is_dead_)
                {
                    y_ -= 5;
                }
                break;

            case 's':
                if (!is_dead_)
                {
                    ++y_;
                }
                break;

            case 'd':
                if (!is_dead_)
                {
                    ++x_;
                }
                break;

            case 'a':
                if (!is_dead_)
                {
                    --x_;
                }
                break;
        }
    }

    void Move() override { ++y_; }

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
