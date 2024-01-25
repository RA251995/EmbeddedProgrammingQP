#include <stdint.h>
#include "delay.h"
#include "stm32f10x.h"                  // Device header

int main(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */
    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */
    
    SysTick->LOAD = SystemCoreClock / 5U - 1U; /* 0.2 seconds */
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE;
    
    
    
    while (1)
    {
    }
    
    // return 0;
}
