#ifndef ISCREEN_H
#define ISCREEN_H

#include "irectangle2d.h"

template <typename Color>
struct IScreen2D : public IRectangle2D
{
    using DeltaVal = unsigned short;

    // Settings
    virtual DeltaVal delta_x() const noexcept = 0;
    virtual DeltaVal delta_y() const noexcept = 0;

    // Drawing
    virtual void Draw(const char symbol) = 0;
    virtual void Update() = 0;

    // Color
    virtual void SetBackgroundColor(const Color &bg_color) = 0;
    virtual void SetForegroundColor(const Color &fg_color) = 0;
    virtual void FlushColor() = 0;

    // Cursor
    virtual void SetCursor(const Coordinate x, const Coordinate y) = 0;
    virtual void SetCursorStartVals() = 0;
    virtual void MoveCursor(const Coordinate delta_x, const Coordinate delta_y) = 0;
    virtual void SetCursorVisible(const bool visisble = false) = 0;

    virtual void Clear() = 0;

    virtual ~IScreen2D() {}
};
#endif  // !ISCREEN_H
