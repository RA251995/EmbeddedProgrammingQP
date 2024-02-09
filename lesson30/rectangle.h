#ifndef __RECTANGLE_H
#define __RECTANGLE_H

#include "shape.h"

typedef struct
{
    Shape super;
    
    uint16_t width;
    uint16_t height;
} Rectangle;

void Rectangle_ctor(Rectangle * const me,
                    int16_t x0, int16_t y0,
                    int16_t w0, int16_t h0);
void Rectangle_draw(Rectangle const * const me);
uint32_t Rectangle_area(Rectangle const * const me);

#endif /* __RECTANGLE_H */