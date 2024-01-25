/* Board Support Package */
#include "stm32f4_cmsis.h"
#include "stm32f10x.h"
#include "core_cm3.h"

void assert_failed(char const *file, int line)
{
    /* TODO: Damage Control */
    NVIC_SystemReset();
}

void SysTick_Handler(void)
{
}
