#include "delay.h"

#define RCC_BASE_ADDR       (0x40021000U)
#define RCC_APB2ENR         *((unsigned int *)(RCC_BASE_ADDR + 0x18U))

#define GPIO_CRL_OFFSET     (0x00U)
#define GPIO_CRH_OFFSET     (0x04U)
// #define GPIO_ODR_OFFSET     (0x0CU)
#define GPIO_BSRR_OFFSET    (0x10U)

#define GPIOA_BASE_ADDR     (0x40010800U)
#define GPIOA_CRL           (*((volatile unsigned int *)(GPIOA_BASE_ADDR + GPIO_CRL_OFFSET)))
// #define GPIOA_ODR           (*((volatile unsigned int *)(GPIOA_BASE_ADDR + GPIO_ODR_OFFSET)))
#define GPIOA_BSRR          (*((volatile unsigned int *)(GPIOA_BASE_ADDR + GPIO_BSRR_OFFSET)))

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)

unsigned fact(unsigned n);

unsigned fact(unsigned n)
{
    if (n == 0U)
    {
        return 1;
    }
    
    return n*fact(n-1);
}

int main(void)
{
    unsigned volatile x;
    
    x = fact(0U);
    x = 2U + 3U*fact(1U);
    (void)fact(5U);
    
    RCC_APB2ENR |= (1U << 2U); /* Enable GPIOC & GPIOA clock */
    GPIOA_CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA_CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */
    
    // GPIOA_ODR |= LED_ORANGE;
    GPIOA_BSRR = LED_ORANGE;
    while (1)
    {
        // GPIOA_ODR |= LED_GREEN;
        GPIOA_BSRR = LED_GREEN; /* Set LED bit */
        
        delay(1000000U);
        
        // GPIOA_ODR &= ~LED_GREEN;
        GPIOA_BSRR = LED_GREEN << 16; /* Reset LED bit */
        
        delay(500000U);
    }
    
    // return 0;
}
