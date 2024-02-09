#ifndef __RECTANGLE_H
#define __RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape
{
private:
    uint16_t width;
    uint16_t height;

public:
    Rectangle(int16_t x0, int16_t y0,
              int16_t w0, int16_t h0);
    void draw() const;
    uint32_t area() const;
};

#endif /* __RECTANGLE_H */