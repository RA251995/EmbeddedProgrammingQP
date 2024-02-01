/* Board Support Package */
/* Includes */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "qpc.h"
#include "bsp.h"

/* Function Prototypes */
void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void);
void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void);
void SysTick_Handler(void);

/* Global Functions */
void SysTick_Handler(void)
{
    GPIOA->BSRR = TEST_PIN1;
    QXK_ISR_ENTRY();

    QF_TICK_X(0U, (void *)0);

    QXK_ISR_EXIT();
    GPIOA->BRR = TEST_PIN1;
}

void EXTI9_5_IRQHandler(void);
void EXTI9_5_IRQHandler(void)
{
    QXK_ISR_ENTRY();

    if ((EXTI->PR & EXTI_PR_PR5) != 0U)
    {
        QXSemaphore_signal(&SW1_sem);
        EXTI->PR |= EXTI_PR_PR5; /* Clear EXTI5 pending bit */
    }
    else
    {
        /* Un-handled interrupt */
        Q_onAssert("EXTI9_5_IRQHandler", __LINE__);
    }

    QXK_ISR_EXIT();
}

void BSP_init(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */

    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U) | (0x1U << 12U)
            | (0x1U << 16U)); /* Set PA0..4 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U) | (0x3U << 14U)
            | (0x3U << 18U)); /* Set PA0..4 configuration to push-pull */

    /* Set PA5 configuration to input with pull-up / pull-down */
    GPIOA->CRL &= ~(0x3U << 22U);
    GPIOA->CRL |= (0x2U << 22U);
    /* Enable pull-up on PA5 */
    GPIOA->ODR |= (0x1U << 5U);

    /* Enable interrupt on PA5 falling edge */
    EXTI->IMR |= EXTI_IMR_MR5; /* Un-mask EXTI5 line interrupt */
    EXTI->FTSR |= EXTI_FTSR_TR5; /* Select falling edge trigger for EXTI5 */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; /* Enable AFIO clock */
    AFIO->EXTICR[1] |= AFIO_EXTICR1_EXTI0_PA; /* Map EXTI5 to Port A */
}

void QF_onStartup(void)
{
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); /* TODO: Assert return */

    NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI); /* Set SysTick interrupt priority (highest kernal-aware)*/
    NVIC_SetPriority(EXTI9_5_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1U);

    NVIC_EnableIRQ(EXTI9_5_IRQn); /* Enable EXTI9-5 interrupt in NVIC */
}

void QF_onCleanup(void)
{

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

void QXK_onIdle(void)
{
    GPIOA->BSRR = TEST_PIN2;
    GPIOA->BRR = TEST_PIN2;
//    __WFI();
}

void __NO_RETURN __attribute__ ((naked)) Q_onAssert(const char *module,
        const int id)
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
