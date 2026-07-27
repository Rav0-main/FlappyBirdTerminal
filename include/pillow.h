#ifndef PILLOW_H
#define PILLOW_H

#include "bird.h"
#include "icollision.h"
#include "idrawable.h"
#include "screen.h"

class Pillow : public ICollision, public IDrawable<TerminalColor>
{
   public:
    bool HasCollisionWith(const ICollision &other) const override;
    bool HasCollisionWith(const Bird &bird) const;

    void DrawOn(IScreen2D<TerminalColor> &screen) const override;
};

#endif  // !PILLOW_H
