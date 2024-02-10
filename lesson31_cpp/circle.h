#ifndef __CIRCLE_H
#define __CIRCLE_H

#include "shape.h"

class Circle : public Shape
{
private:
    uint16_t radius;

public:
    Circle(int16_t x0, int16_t y0,
           uint16_t r0);
    virtual void draw() const;
    virtual uint32_t area() const;
};

#endif /* __CIRCLE_H */