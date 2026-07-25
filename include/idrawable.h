#ifndef IDRAWABLE_H
#define IDRAWABLE_H

struct IDrawable
{
    virtual void Draw() const noexcept = 0;
    virtual ~IDrawable() {}
};

#endif  // !IDRAWABLE_H
