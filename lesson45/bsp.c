#include "stm32f10x.h"
#include "bsp.h"      /* Board Support Package */
#include "my_printf.h"

#define LED_GREEN           (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_ORANGE          (0x1U << 2U)
#define TEST_PIN1           (0x1U << 3U)
#define TEST_PIN2           (0x1U << 4U)
#define BTN_SW1             (0x1U << 5U)
#define BTN_SW2             (0x1U << 6U)

/* ISRs ====================================================================*/
void SysTick_Handler(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current;
    uint32_t tmp;
    
    GPIOA->BSRR = TEST_PIN1;
    
    QF_TICK_X(0U, (void *)0); /* Process all QP/C time QEvts */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~GPIOA->IDR; /* read SW1 */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_SW1) != 0U) {  /* debounced SW1 state changed? */
        if ((buttons.depressed & BTN_SW1) != 0U) { /* is SW1 depressed? */
            /* post the "button-press" QEvt from ISR */
            static QEvt const buttonPressedEvt
                = QEVT_INITIALIZER(BUTTON_PRESSED_SIG);
            QACTIVE_POST(AO_TimeBomb, &buttonPressedEvt, 0U);
            MY_PRINTF("BTN %s %d\n", "SW1", 1);
        }
        else { /* the button is released */
            /* post the "button-release" QEvt from ISR */
            static QEvt const buttonReleasedEvt
                = QEVT_INITIALIZER(BUTTON_RELEASED_SIG);
            QACTIVE_POST(AO_TimeBomb, &buttonReleasedEvt, 0U);
            MY_PRINTF("BTN %s %d\n", "SW1", 0);
        }
    }
    if ((tmp & BTN_SW2) != 0U) {  /* debounced SW1 state changed? */
        if ((buttons.depressed & BTN_SW2) != 0U) { /* is SW1 depressed? */
            /* post the "button2-press" QEvt from ISR */
            static QEvt const button2PressedEvt
                = QEVT_INITIALIZER(BUTTON2_PRESSED_SIG);
            QACTIVE_POST(AO_TimeBomb, &button2PressedEvt, 0U);
            MY_PRINTF("BTN %s %d\n", "SW2", 1);
        }
        else { /* the button is released */
            /* post the "button2-release" QEvt from ISR */
            static QEvt const button2ReleasedEvt
                = QEVT_INITIALIZER(BUTTON2_RELEASED_SIG);
            QACTIVE_POST(AO_TimeBomb, &button2ReleasedEvt, 0U);
            MY_PRINTF("BTN %s %d\n", "SW2", 0);
        }
    }
    
    GPIOA->BRR = TEST_PIN1;
}
/*..........................................................................*/
void QV_onIdle(void) {
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    QV_CPU_SLEEP();
#else
    QF_INT_ENABLE();
#endif
}


/* BSP functions ===========================================================*/
void BSP_init(void)
{
    /* GPIOA configuration */
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */

    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U) | (0x1U << 12U)
            | (0x1U << 16U)); /* Set PA0..4 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U) | (0x3U << 14U)
            | (0x3U << 18U)); /* Set PA0..4 configuration to push-pull */

    /* Set PA5..6 configuration to input with pull-up / pull-down */
    GPIOA->CRL &= ~((0x3U << 22U) | (0x3U << 26U));
    GPIOA->CRL |= ((0x2U << 22U) | (0x2U << 26U));
    /* Enable pull-up on PA5..6 */
    GPIOA->ODR |= ((0x1U << 5U) | (0x1U << 6U));
    
    MY_PRINTF_INIT();
}

void QF_onStartup(void)
{
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
     */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system, see NOTE1 */
    NVIC_SetPriority(SysTick_IRQn,  QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs in the NVIC... */
    /* ... */
}

void QF_onCleanup(void)
{
    
}

void BSP_ledGreenOn(void)
{
    GPIOA->BSRR = LED_GREEN;
    MY_PRINTF("LED-%s is %d\n", "green", 1);
}

void BSP_ledGreenOff(void)
{
    GPIOA->BRR = LED_GREEN;
    MY_PRINTF("LED-%s is %d\n", "green", 0);
}

void BSP_ledYellowOn(void)
{
    GPIOA->BSRR = LED_YELLOW;
    MY_PRINTF("LED-%s is %d\n", "yellow", 1);
}

void BSP_ledYellowOff(void)
{
    GPIOA->BRR = LED_YELLOW;
    MY_PRINTF("LED-%s is %d\n", "yellow", 0);
}

void BSP_ledOrangeOn(void)
{
    GPIOA->BSRR = LED_ORANGE;
    MY_PRINTF("LED-%s is %d\n", "orange", 1);
}

void BSP_ledOrangeOff(void)
{
    GPIOA->BRR = LED_ORANGE;
    MY_PRINTF("LED-%s is %d\n", "orange", 0);
}

void __NO_RETURN Q_onAssert(char const * const module, int const id) 
{
    (void)module; /* unused parameter */
    (void)id;     /* unused parameter */
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

#ifdef SPY /* Tracing enabled? */
int fputc(int c, FILE *stream)
{
    (void)stream; /* unused parameter */
    
    GPIOA->BSRR = TEST_PIN2;
    
    //ITM_SendChar(c);
    while (!(USART3->SR & USART_SR_TXE));
    USART3->DR = c;
    
    GPIOA->BRR = TEST_PIN2;
    
    return c;
}

void printf_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; /* Enable GPIOB clock */
    /* Configure alternate function for PB10(TX) and PB11(RX) */
    /* Set PB10 mode to output (50 MHz) */
    GPIOB->CRH |= (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1);
    /* Set PB10 configuartion to alternate function output push-pull */
    GPIOB->CRH &= ~GPIO_CRH_CNF10;
    GPIOB->CRH |= GPIO_CRH_CNF10_1;
    
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; /* Enable USART3 clock */
//    /* Set baud rate to 9600 bps
//     * baud = 36e6 / (16 * USARTDIV) => USARTDIV = 234.375
//     * DIV_Fraction = 16*0.5 = 8, DIV_Mantissa = 19 */
//    USART3->BRR = 0xEA6;
     /* Set baud rate to 115200 bps
      * DIV_Fraction = 16*0.375 = 8, DIV_Mantissa = 234 */
    USART3->BRR = 0x138;
    /* Enable USART */
    USART3->CR1 |= USART_CR1_UE; /* USART enable */
    USART3->CR1 |= USART_CR1_TE; /* Transmitter enable */
}
#endif /* SPY */
