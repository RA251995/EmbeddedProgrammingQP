#include "rectangle.h"

Rectangle::Rectangle(int16_t x0, int16_t y0,
                     int16_t w0, int16_t h0)
: Shape(x0, y0),
  width(w0),
  height(h0)
{
}

void Rectangle::draw() const
{
    // TODO
}

uint32_t Rectangle::area() const
{
    return (uint32_t)width * (uint32_t)height;
}
