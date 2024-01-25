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

/* Variable Declarations */
static uint32_t volatile l_tickCtr;

/* Global Functions */
void BSP_init(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */
    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */

    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); /* TODO: Assert return */

    __enable_irq();
}

void SysTick_Handler(void)
{
    ++l_tickCtr;
}

uint32_t BSP_tickCtr(void)
{
    uint32_t tickCtr;

    __disable_irq();
    tickCtr = l_tickCtr;
    __enable_irq();

    return tickCtr;
}

void BSP_delay(uint32_t ticks)
{
    uint32_t start = BSP_tickCtr(); /* Not portable */
    while ((BSP_tickCtr() - start) < ticks)
    {
    }
}

void BSP_ledGreenOn(void)
{
    GPIOA->BSRR = LED_GREEN;
}

void BSP_ledGreenOff(void)
{
    GPIOA->BRR = LED_GREEN;
}

void BSP_ledYellowOn(void)
{
    GPIOA->BSRR = LED_YELLOW;
}
void BSP_ledYellowOff(void)
{
    GPIOA->BRR = LED_YELLOW;
}

void BSP_ledOrangeOn(void)
{
    GPIOA->BSRR = LED_ORANGE;
}
void BSP_ledOrangeOff(void)
{
    GPIOA->BRR = LED_ORANGE;
}

void assert_failed(__attribute__ ((unused)) const char *file,
        __attribute__ ((unused)) int32_t line)
{
    /* TODO: Damage Control */
    NVIC_SystemReset();
}

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
