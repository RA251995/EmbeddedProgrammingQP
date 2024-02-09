#include "rectangle.h"

void Rectangle_ctor(Rectangle * const me,
                    int16_t x0, int16_t y0,
                    int16_t w0, int16_t h0)
{
    Shape_ctor(&me->super, x0, y0);
    
    me->width = w0;
    me->height = h0;
}

void Rectangle_draw(Rectangle const * const me)
{
    // TODO
}

uint32_t Rectangle_area(Rectangle const * const me)
{
    return (uint32_t)me->width * (uint32_t)me->height;
}
