#include "qassert.h"
#include "shape.h"
#include <stdlib.h>

Q_DEFINE_THIS_FILE

static Shape s1(1, 2); /* static allocation */

int main(void)
{
    Shape s2(3, 4); /* automatic allocation */
    Shape *ps3 = new Shape(5, 6); /* dynamic allocation */

    s1.moveBy(7, 8);
    s2.moveBy(9, 10);
    ps3->moveBy(-1, -2);

    Q_ASSERT(s1.distanceFrom(&s1) == 0U);
    Q_ASSERT(s1.distanceFrom(&s2) == s2.distanceFrom(&s1));
    Q_ASSERT(
            s1.distanceFrom(&s2)
                    <= s1.distanceFrom(ps3)
                            + ps3->distanceFrom(&s2));

    delete ps3;
    
    return 0;
}

void Q_onAssert(const char *module, const int id)
{
    (void) module; /* Unused parameter */
    (void) id; /* Unused parameter */
#ifndef NDEBUG
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
#endif
    /* TODO: Damage Control */
}
