/* Board Support Package */
#include "stm32f4_cmsis.h"
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"

void __attribute__((noreturn)) assert_failed(void)
{
    /* TODO: Damage Control */
    NVIC_SystemReset();
}

void SysTick_Handler(void)
{
    GPIOA->ODR ^= LED_GREEN;
}
