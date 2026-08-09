#ifndef IMOVABLE_H
#define IMOVABLE_H

struct IMovable
{
    virtual void Move(const double delta_secs_from_last_frame) = 0;
};

#endif  // !IMOVABLE_H
