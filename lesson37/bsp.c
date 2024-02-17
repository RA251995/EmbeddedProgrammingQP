/* Board Support Package */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"

#define LED_GREEN           (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_ORANGE          (0x1U << 2U)
#define BTN_SW1             (0x1U << 5U)

static uint32_t volatile l_tickCtr;

void BSP_init(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */

    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..4 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */
    
    /* Set PA5 configuration to input with pull-up / pull-down */
    GPIOA->CRL &= ~(0x3U << 22U);
    GPIOA->CRL |= (0x2U << 22U);
    /* Enable pull-up on PA5 */
    GPIOA->ODR |= (0x1U << 5U);
    
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

uint8_t BSP_SW1(void)
{
    uint32_t sw1;
    
    __disable_irq();
    sw1 = GPIOA->IDR & BTN_SW1;
    __enable_irq();
    
    return (sw1 != 0U) ? 1U: 0U;
}
