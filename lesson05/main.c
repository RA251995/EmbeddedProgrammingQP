#define RCC_BASE_ADDR       (0x40021000U)
#define RCC_APB2ENR         *((unsigned int *)(RCC_BASE_ADDR + 0x18U))

#define GPIOC_BASE_ADDR     (0x40011000U)
#define GPIOC_CRH           (*((volatile unsigned int *)(GPIOC_BASE_ADDR + 0x04U)))
#define GPIOC_BSRR          (*((volatile unsigned int *)(GPIOC_BASE_ADDR + 0x10U)))
    
void delay(void);

int main(void) 
{
    RCC_APB2ENR |= (1U << 4U); /* Enable GPIOC clock */
    GPIOC_CRH |= (0x01U << 20U); /* Set PC13 mode to output */
    
    while (1)
    {
        GPIOC_BSRR |= (1U << 29U); /* Turn on PC13 LED (Active Low) */
        
        delay();
        
        GPIOC_BSRR |= (1U << 13U); /* Turn off PC13 LED (Active Low) */
        
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
