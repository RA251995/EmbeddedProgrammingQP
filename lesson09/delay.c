#include "delay.h"

void delay(unsigned int iter)
{
    volatile unsigned int counter = 0U;
    while (counter < iter)
    {
        counter++;
    }
}
