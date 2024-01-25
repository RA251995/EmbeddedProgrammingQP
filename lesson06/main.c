#define RCC_BASE_ADDR       (0x40021000U)
#define RCC_APB2ENR         *((unsigned int *)(RCC_BASE_ADDR + 0x18U))

#define GPIO_CRL_OFFSET     (0x00U)
#define GPIO_CRH_OFFSET     (0x04U)
#define GPIO_ODR_OFFSET     (0x0CU)
// #define GPIO_BSRR_OFFSET    (0x10U)

#define GPIOA_BASE_ADDR     (0x40010800U)
#define GPIOA_CRL           (*((volatile unsigned int *)(GPIOA_BASE_ADDR + GPIO_CRL_OFFSET)))
#define GPIOA_ODR           (*((volatile unsigned int *)(GPIOA_BASE_ADDR + GPIO_ODR_OFFSET)))
// #define GPIOA_BSRR          (*((volatile unsigned int *)(GPIOA_BASE_ADDR + GPIO_BSRR_OFFSET)))

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)

void delay(void);

int main(void)
{
    RCC_APB2ENR |= (1U << 2U); /* Enable GPIOC & GPIOA clock */
    GPIOA_CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA_CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */
    
    GPIOA_ODR |= LED_ORANGE;
    while (1)
    {
        GPIOA_ODR |= LED_GREEN;
        
        delay();
        
        GPIOA_ODR &= ~LED_GREEN;
        
        delay();
    }
    
    // return 0;
}

void delay(void)
{
    volatile unsigned int counter = 0U;
    while (counter < 1000000U)
    {
        counter++;
    }
}
