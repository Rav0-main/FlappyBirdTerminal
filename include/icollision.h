#ifndef ICOLLISION_H
#define ICOLLISION_H

struct ICollision
{
    virtual bool HasCollisionWith(const ICollision &other) const = 0;
    virtual ~ICollision() {}
};

#endif  // !ICOLLISION_H
