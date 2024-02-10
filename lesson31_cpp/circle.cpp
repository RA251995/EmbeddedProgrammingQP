#include "circle.h"

Circle::Circle(int16_t x0, int16_t y0,
               uint16_t r0)
: Shape(x0, y0),
  radius(r0)
{
}

void Circle::draw() const
{
    // TODO
}

uint32_t Circle::area() const
{
    return 3U * (uint32_t)radius * (uint32_t)radius;
}
