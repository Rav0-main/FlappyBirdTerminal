#include "bird.h"

bool Bird::HasCollisionWith(const ICollision &other) const
{
    return other.HasCollisionWith(*this);
}

void Bird::DrawOn(IScreen2D<TerminalColor> &screen) const
{
    screen.SetForegroundColor(fg_color_);
    screen.SetCursor(x_, y_);
    for (const char ch : picture_)
    {
        screen.Draw(ch);
    }
}
