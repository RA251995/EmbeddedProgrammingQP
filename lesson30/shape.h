#ifndef __SHAPE_H
#define __SHAPE_H

#include <stdint.h>

typedef struct
{
    int16_t x;
    int16_t y;
} Shape;

void Shape_ctor(Shape *const me, int16_t x0, int16_t y0);
void Shape_moveBy(Shape *const me, int16_t dx, int16_t dy);
uint16_t Shape_distanceFrom(Shape const *const me, Shape const *const other);

#endif /* __SHAPE_H */
