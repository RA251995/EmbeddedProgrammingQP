#include <stdint.h>
#include "delay.h"
#include "stm32f10x.h"                  // Device header

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)

int16_t x = -1;
uint32_t y = LED_ORANGE | LED_GREEN;
int16_t sqr[] = {
    1*1,
    2*2,
    3*3,
    4*4
};

typedef struct {
    uint8_t x;
    uint16_t y;
} Point;

static Point p1 = {
    123U,
    0x1234U
};

typedef struct {
    Point top_left;
    Point bottom_right;
} Window;

static Window w1 = {
    { 123U, 0x1234U },
    { 234U, 0x6789U }
};

int main(void)
{
    p1.x += 1;
    w1.top_left.x += 1;
    
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */
    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */
    
    GPIOA->BSRR = LED_ORANGE;
    while (1)
    {
        GPIOA->BSRR = LED_GREEN; /* Set LED bit */
        
        delay(1000000U);
        
        GPIOA->BSRR = LED_GREEN << 16; /* Reset LED bit */
        
        delay(1000000U / 2U);
    }
    
    // return 0;
}
