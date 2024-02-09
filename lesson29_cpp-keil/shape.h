#ifndef __SHAPE_H
#define __SHAPE_H

#include <stdint.h>

class Shape
{
private:
    int16_t x;
    int16_t y;

public:
    Shape(int16_t x0, int16_t y0);
    void moveBy(int16_t dx, int16_t dy);
    uint16_t distanceFrom(Shape const *const other) const;
};

#endif /* __SHAPE_H */
