#include <stdint.h>
#include "delay.h"
#include "stm32f10x.h"                  // Device header

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)

typedef struct /* __attribute__((packed)) */
{
    uint8_t y;
    uint16_t x;
} Point;

static Point p1, p2;

typedef struct {
    Point top_left;
    Point bottom_right;
} Window;

typedef struct {
    Point corners[3];
} Triangle;

Window w1, w2;
Triangle t;

int main(void)
{
    Point *pp;
    Window *wp;
    
    p1.x = sizeof(Point);
    p1.y = 0xAAU;
    
    w1.top_left.x = 1U;
    w1.bottom_right.y = 2U;
    
    t.corners[0].x = 1U;
    t.corners[2].y = 2U;
    
    p2 = p1;
    w2 = w1;
    
    pp = &p1;
    wp = &w2;
    
    (*pp).x = 1U;
    (*wp).top_left = *pp;
    
    pp->x = 1U;
    wp->top_left = *pp;
    
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
