#ifndef ISCREEN_H
#define ISCREEN_H

#include "idrawable.h"

template <typename Color>
struct IScreen2D
{
    using SizeParam = unsigned short int;
    using Coordinate = short int;
    using DeltaVal = unsigned short int;

    // Settings
    virtual SizeParam width() const noexcept = 0;
    virtual SizeParam height() const noexcept = 0;
    virtual DeltaVal delta_x() const noexcept = 0;
    virtual DeltaVal delta_y() const noexcept = 0;
    virtual Coordinate start_val_x() const noexcept = 0;
    virtual Coordinate start_val_y() const noexcept = 0;

    // Drawing
    virtual void Draw(const char symbol) = 0;
    virtual void Draw(const IDrawable &shape) = 0;
    virtual void Update() = 0;

    // Color
    virtual void SetBackgroundColor(const Color &bg_color) = 0;
    virtual void SetForegroundColor(const Color &fg_color) = 0;
    // virtual void FlushColor() = 0;

    // Cursor
    virtual void SetCursor(const Coordinate x, const Coordinate y) = 0;
    virtual void SetCursorStartVals() noexcept = 0;
    // virtual void MoveCursor(const Coordinate delta_x, const Coordinate delta_y) = 0;
    virtual void SetCursorVisible(const bool visisble = false) noexcept = 0;

    virtual void Clear() noexcept = 0;

    virtual ~IScreen2D() {}
};
#endif  // !ISCREEN_H
