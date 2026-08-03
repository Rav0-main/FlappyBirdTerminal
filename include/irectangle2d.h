#ifndef IRECTANGLE2D_H
#define IRECTANGLE2D_H

struct IRectangle2D
{
    using Coordinate = short;
    using SizeParam = unsigned short;

    virtual Coordinate start_val_x() const noexcept = 0;
    virtual Coordinate start_val_y() const noexcept = 0;
    virtual Coordinate end_val_x() const noexcept = 0;
    virtual Coordinate end_val_y() const noexcept = 0;
    virtual SizeParam width() const noexcept = 0;
    virtual SizeParam height() const noexcept = 0;

    virtual ~IRectangle2D() {}
};

#endif  // !IRECTANGLE2D_H
