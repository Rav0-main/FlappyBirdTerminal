#ifndef BIRD_H
#define BIRD_H

#include <stdexcept>
#include "icollision.h"
#include "idrawable.h"
#include "iscreen2d.h"

template <typename Color>
class Bird : public ICollision, public IDrawable<Color>
{
   private:
    IScreen2D<Color>::Coordinate x_, y_;
    bool is_dead_;
    const std::string picture_;
    const Color fg_color_;

   public:
    size_t length() const noexcept { return picture_.length(); }
    bool is_alive() const noexcept { return !is_dead_; }

    // :(
    void Kill() noexcept { is_dead_ = true; }
    void set_x(const IScreen2D<Color>::Coordinate new_x) noexcept { x_ = new_x; }
    void set_y(const IScreen2D<Color>::Coordinate new_y) noexcept { y_ = new_y; }
    IScreen2D<Color>::Coordinate x() const noexcept { return x_; }
    IScreen2D<Color>::Coordinate y() const noexcept { return y_; }

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
        if (picture_.length() < 2)
        {
            throw std::out_of_range("Picture length must be >= 2.");
        }
    }

    void HandlePressedKey(const int key)
    {
        switch (key)
        {
            case 'w':
                if (!is_dead_)
                {
                    --y_;
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
