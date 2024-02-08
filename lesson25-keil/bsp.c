/* Board Support Package */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"
#include "miros.h"

#define LED_GREEN           (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_ORANGE          (0x1U << 2U)
#define TEST_PIN1           (0x1U << 3U)
#define TEST_PIN2           (0x1U << 4U)

void SysTick_Handler(void);
void SysTick_Handler(void)
{
    GPIOA->BSRR = TEST_PIN1;

    OS_tick();

    __disable_irq();
    OS_sched();
    __enable_irq();

    GPIOA->BRR = TEST_PIN1;
}

void BSP_init(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */
    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U) | (0x1U << 12U)
            | (0x1U << 16U)); /* Set PA0..4 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U) | (0x3U << 14U)
            | (0x3U << 18U)); /* Set PA0..4 configuration to push-pull */
}

void OS_onStartup(void)
{
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); /* TODO: Assert return */

    /* Set SysTick interrupt priority (highest) */
    NVIC_SetPriority(SysTick_IRQn, 0U);
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

void OS_onIdle(void)
{
    GPIOA->BSRR = TEST_PIN2;
    GPIOA->BRR = TEST_PIN2;
    __WFI();
}

void Q_onAssert(char const * const module, int const id);
void __NO_RETURN Q_onAssert(char const * const module, int const id) 
{
    (void)module; // unused parameter
    (void)id;     // unused parameter
#ifndef NDEBUG
    /* Light up all LEDs */
    GPIOA->ODR = LED_GREEN | LED_ORANGE | LED_YELLOW;
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
#else
    NVIC_SystemReset();
#endif
}
