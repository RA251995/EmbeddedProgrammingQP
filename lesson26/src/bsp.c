/* Board Support Package */
/* Includes */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"
#include "miros.h"

/* Function Prototypes */
void __NO_RETURN __attribute__ ((naked)) NMI_Handler(void);
void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void);
void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void);
void SysTick_Handler(void);

/* Global Functions */
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
//    __WFI();
}

void __NO_RETURN __attribute__ ((naked)) Q_onAssert(const char *module,
        int32_t id)
{
    /* TODO: Write naked function in assembly */

    (void) module; /* Unused parameter */
    (void) id; /* Unused parameter */
#ifndef NDEBUG
    /* Light up all LEDs */
    GPIOA->ODR = LED_GREEN | LED_ORANGE | LED_YELLOW;
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
#endif
    /* TODO: Damage Control */
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) NMI_Handler(void)
{
    Q_onAssert("NMI", __LINE__);
}

void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void)
{
    Q_onAssert("HardFault", __LINE__);
}

void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void)
{
    Q_onAssert("MemManage", __LINE__);
}

void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void)
{
    Q_onAssert("BusFault", __LINE__);
}

void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void)
{
    Q_onAssert("UsageFault", __LINE__);
}
