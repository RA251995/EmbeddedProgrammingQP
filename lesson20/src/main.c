/* Includes */
#include "bsp.h"
#include "delay.h"
#include "stm32f10x.h"
#include <stdint.h>

/* Global Functions */
int32_t main(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */
    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */

    SysTick->LOAD = SystemCoreClock / 5U - 1U; /* 0.2 seconds */
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT
            | SysTick_CTRL_ENABLE;

    SysTick_Handler();

    __enable_irq();

    while (1)
    {
        GPIOA->BSRR = LED_YELLOW;
        GPIOA->BRR = LED_YELLOW;
//        __disable_irq();
//        GPIOA->ODR |= LED_YELLOW;
//        __enable_irq();
//        __disable_irq();
//        GPIOA->ODR &= ~LED_YELLOW;
//        __enable_irq();
    }

    return 0;
}
