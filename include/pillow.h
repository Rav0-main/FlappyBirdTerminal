#ifndef PILLOW_H
#define PILLOW_H

#include <stdexcept>
#include <tuple>
#include "bird.h"
#include "icollision.h"
#include "idrawable.h"
#include "iscreen2d.h"
#include "screen.h"

class Pillow : public ICollision, public IDrawable<TerminalColor>
{
   private:
    // Coordinates of left up vertex.
    IScreen2D<TerminalColor>::Coordinate x_, y_;
    const IScreen2D<TerminalColor>::SizeParam width_;

    const IScreen2D<TerminalColor>::SizeParam non_empty_up_height_, empty_height_,
        non_empty_down_height_;

    const char start_symbol_, middle_symbol_, end_symbol_;
    const TerminalColor fg_color_;

   public:
    Pillow(const std::pair<IScreen2D<TerminalColor>::Coordinate,
                           IScreen2D<TerminalColor>::Coordinate> &start_coords,
           const IScreen2D<TerminalColor>::SizeParam width,
           const std::pair<IScreen2D<TerminalColor>::SizeParam, IScreen2D<TerminalColor>::SizeParam>
               &non_empty_heights,
           const IScreen2D<TerminalColor>::SizeParam empty_height,
           const std::tuple<char, char, char> &symbols,
           const TerminalColor &color)
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
    bool HasCollisionWith(const ICollision &other) const override;
    bool HasCollisionWith(const Bird &other) const;

    void DrawOn(IScreen2D<TerminalColor> &screen) const override;

    ~Pillow() override = default;
};

#endif  // !PILLOW_H
