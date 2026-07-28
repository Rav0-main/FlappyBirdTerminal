#ifndef BIRD_H
#define BIRD_H

#include <stdexcept>
#include "icollision.h"
#include "idrawable.h"
#include "iscreen2d.h"
#include "screen.h"

class Bird : public ICollision, public IDrawable<TerminalColor>
{
   private:
    IScreen2D<TerminalColor>::Coordinate x_, y_;
    const std::string picture_;
    const TerminalColor fg_color_;

   public:
    size_t length() const noexcept { return picture_.length(); }
    void set_x(const IScreen2D<TerminalColor>::Coordinate new_x) noexcept { x_ = new_x; }
    void set_y(const IScreen2D<TerminalColor>::Coordinate new_y) noexcept { y_ = new_y; }
    IScreen2D<TerminalColor>::Coordinate x() const noexcept { return x_; }
    IScreen2D<TerminalColor>::Coordinate y() const noexcept { return y_; }

    Bird(const std::pair<IScreen2D<TerminalColor>::Coordinate, IScreen2D<TerminalColor>::Coordinate>
             &start_coords,
         const std::string &picture,
         const TerminalColor &color)
        : x_(start_coords.first), y_(start_coords.second), picture_(picture), fg_color_(color)
    {
        if (picture_.length() != 2)
        {
            throw std::out_of_range("Picture length must be == 2.");
        }
    }

    bool HasCollisionWith(const ICollision &other) const override;
    void DrawOn(IScreen2D<TerminalColor> &screen) const override;

    ~Bird() override = default;
};

#endif  // !COLLISION_H
