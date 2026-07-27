#ifndef BIRD_H
#define BIRD_H

#include "config.h"
#include "icollision.h"
#include "idrawable.h"
#include "iscreen2d.h"
#include "screen.h"

class Bird : public ICollision, public IDrawable<TerminalColor>
{
   private:
    IScreen2D<TerminalColor>::Coordinate x_, y_;
    const std::string picture_ = BIRD_PICTURE;
    const TerminalColor fg_color_;

   public:
    std::string picture() const noexcept { return picture_; }
    void set_x(const IScreen2D<TerminalColor>::Coordinate new_x) noexcept { x_ = new_x; }
    void set_y(const IScreen2D<TerminalColor>::Coordinate new_y) noexcept { y_ = new_y; }
    IScreen2D<TerminalColor>::Coordinate x() const noexcept { return x_; }
    IScreen2D<TerminalColor>::Coordinate y() const noexcept { return y_; }

    Bird(const IScreen2D<TerminalColor>::Coordinate x,
         const IScreen2D<TerminalColor>::Coordinate y,
         const TerminalColor &color)
        : x_(x), y_(y), fg_color_(color)
    {
    }

    bool HasCollisionWith(const ICollision &other) const override;
    void DrawOn(IScreen2D<TerminalColor> &screen) const override;

    ~Bird() override = default;
};

#endif  // !COLLISION_H
