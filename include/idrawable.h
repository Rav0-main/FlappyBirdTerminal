#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include "iscreen2d.h"

template <typename Color>
struct IDrawable
{
    virtual void DrawOn(IScreen2D<Color> &screen) const = 0;
    virtual ~IDrawable() {}
};

#endif  // !IDRAWABLE_H
