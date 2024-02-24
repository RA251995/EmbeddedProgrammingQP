#include "et.h" /* ET: embedded test */

#include "stm32f10x.h"

///* Local-scope objects -----------------------------------------------------*/
#define LED_GREEN           (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_ORANGE          (0x1U << 2U)

#define TEST_PIN1           (0x1U << 3U)
#define TEST_PIN2           (0x1U << 4U)

#define BTN_SW1             (0x1U << 5U)
#define BTN_SW2             (0x1U << 6U)

///*..........................................................................*/
void ET_onInit(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    /* 
     * Configure LED, button and test pins 
     */
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

    /* 
     * Configure USART3 
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; /* Enable GPIOB clock */
    /* Configure alternate function for PB10(TX) and PB11(RX) */
    /* Set PB10 mode to output (50 MHz) */
    GPIOB->CRH |= (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1);
    /* Set PB10 configuartion to alternate function output push-pull */
    GPIOB->CRH &= ~GPIO_CRH_CNF10;
    GPIOB->CRH |= GPIO_CRH_CNF10_1;
    
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; /* Enable USART3 clock */
     /* Set baud rate to 115200 bps
      * DIV_Fraction = 16*0.375 = 8, DIV_Mantissa = 234 */
    USART3->BRR = 0x138;
    /* Enable USART */
    USART3->CR1 |= USART_CR1_UE; /* USART enable */
    USART3->CR1 |= USART_CR1_TE; /* Transmitter enable */
}
/*..........................................................................*/
void ET_onPrintChar(char const ch) {
    while (!(USART3->SR & USART_SR_TXE));
    USART3->DR = ch;
}
///*..........................................................................*/
void ET_onExit(int err) {
    (void)err;
    /* Light up all LEDs */
    GPIOA->ODR = LED_GREEN | LED_ORANGE | LED_YELLOW;
    /* blink the on-board LED2... */
    for (;;) {
        unsigned volatile ctr;
        GPIOA->ODR ^= LED_GREEN;
        for (ctr = 200000U; ctr != 0U; --ctr) {}
    }
}

/*..........................................................................*/
/* fault handler called from the exception handlers in the startup code */
void assert_failed(char const * const module, int const loc) {
    (void)module;
    (void)loc;
    ET_onExit(-1);
}
