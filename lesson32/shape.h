#ifndef __SHAPE_H
#define __SHAPE_H

#include <stdint.h>

typedef struct
{
    struct ShapeVtable const *vptr;
    
    int16_t x;
    int16_t y;
} Shape;

struct ShapeVtable
{
    void (*draw)(Shape const * const me);
    uint32_t (*area)(Shape const * const me);
};

void Shape_ctor(Shape *const me, int16_t x0, int16_t y0);
void Shape_moveBy(Shape *const me, int16_t dx, int16_t dy);
uint16_t Shape_distanceFrom(Shape const *const me, Shape const *const other);

static inline void Shape_draw_vcall(Shape const * const me)
{
    (*me->vptr->draw)(me);
}

static inline uint32_t Shape_area_vcall(Shape const * const me)
{
    return (*me->vptr->area)(me);
}

//#define SHAPE_DRAW_VCALL(me) (*(me)->vptr->draw)((me))
//#define SHAPE_AREA_VCALL(me) (*(me)->vptr->area)((me))

void drawGraph(Shape const *graph[]);

#endif /* __SHAPE_H */
