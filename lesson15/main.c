#include <stdint.h>
#include "delay.h"
#include "stm32f10x.h"                  // Device header

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)

int main(void)
{
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
