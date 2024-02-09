/* Board Support Package */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "qpc.h"
#include "bsp.h"

#define LED_GREEN           (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_ORANGE          (0x1U << 2U)
#define TEST_PIN1           (0x1U << 3U)
#define TEST_PIN2           (0x1U << 4U)
#define SW1_PIN             (0x1U << 5U)

// SEM
//static QXSemaphore Morse_sem;
// MUTEX
static QXMutex Morse_mutex;

void SysTick_Handler(void)
{
    GPIOA->BSRR = TEST_PIN1;
    QXK_ISR_ENTRY();

    QF_TICK_X(0U, (void *)0);

    QXK_ISR_EXIT();
    GPIOA->BRR = TEST_PIN1;
}

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
// SEM
//    QXSemaphore_init(&Morse_sem, 1U, 1U);
// MUTEX
    QXMutex_init(&Morse_mutex, 6U);
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

void BSP_ledGreenToggle(void)
{
    QF_CRIT_ENTRY();
    GPIOA->ODR ^= LED_GREEN;
    QF_CRIT_EXIT();
}

void BSP_ledYellowOn(void)
{
    GPIOA->BSRR = LED_YELLOW;
}

void BSP_ledYellowOff(void)
{
    GPIOA->BRR = LED_YELLOW;
}

void BSP_ledYellowToggle(void)
{
    QF_CRIT_ENTRY();
    GPIOA->ODR ^= LED_YELLOW;
    QF_CRIT_EXIT();
}

void BSP_ledOrangeOn(void)
{
    GPIOA->BSRR = LED_ORANGE;
}

void BSP_ledOrangeOff(void)
{
    GPIOA->BRR = LED_ORANGE;
}

void BSP_ledOrangeToggle(void)
{
    QF_CRIT_ENTRY();
    GPIOA->ODR ^= LED_ORANGE;
    QF_CRIT_EXIT();
}

void BSP_sendMorseCode(uint32_t bitmask)
{
    uint32_t volatile delay_ctr;
// LOCK
//    QSchedStatus sstat;
    enum
    {
        DOT_DELAY = 225U
    };

// SEM
//    QXSemaphore_wait(&Morse_sem, QXTHREAD_NO_TIMEOUT);
// LOCK
//    sstat = QXK_schedLock(5U);
// MUTEX
    QXMutex_lock(&Morse_mutex, QXTHREAD_NO_TIMEOUT);
    for (; bitmask != 0U; bitmask <<= 1)
    {
        if ((bitmask & (1U << 31U)) != 0U)
        {
            BSP_ledGreenOn();
        }
        else
        {
            BSP_ledGreenOff();
        }
        for (delay_ctr = DOT_DELAY; delay_ctr != 0U; --delay_ctr)
        {
        }
    }
    BSP_ledGreenOff();
    for (delay_ctr = 7U * DOT_DELAY; delay_ctr != 0U; --delay_ctr)
    {
    }
// SEM
//    QXSemaphore_signal(&Morse_sem);
// LOCK
//    QXK_schedUnlock(sstat);
// MUTEX
    QXMutex_unlock(&Morse_mutex);
}

void QXK_onIdle(void)
{
    GPIOA->BSRR = TEST_PIN2;
    GPIOA->BRR = TEST_PIN2;
//    __WFI();
}

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
