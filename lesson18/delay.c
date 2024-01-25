#include "delay.h"

void delay(unsigned int volatile iter)
{
    while (iter > 0) /* delay loop */
    {
        --iter;
    }
}
