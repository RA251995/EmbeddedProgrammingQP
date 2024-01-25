/* Board Support Package */
/* Includes */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"

/* Function Prototypes */
void __NO_RETURN __attribute__ ((naked)) NMI_Handler(void);
void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void);
void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void);

/* Global Functions */
void __NO_RETURN __attribute__ ((naked)) NMI_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void)
{
    NVIC_SystemReset();
}

void SysTick_Handler(void)
{
    GPIOA->ODR ^= LED_GREEN;
}

void assert_failed(__attribute__ ((unused)) const char *file,
        __attribute__ ((unused)) int32_t line)
{
    /* TODO: Damage Control */
    NVIC_SystemReset();
}
