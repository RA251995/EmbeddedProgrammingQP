#include <stdint.h>
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

uint8_t u8a, u8b;
uint16_t u16c, u16d;
uint32_t u32e, u32f;
uint64_t u64g;
int32_t s32;

int main(void)
{   
    u8a = sizeof(u8a);
    u16c = sizeof(uint16_t);
    u32e = sizeof(uint32_t);
    
    u8a = 0xa1U;
    u16c = 0xc1c2U;
    u32e = 0xe1e2e3e4U;
    
    u8b = u8a;
    u16d = u16c;
    u32f = u32e;
    
    u32e = 0x90000000U;
    u32f = 0x80000000U;
    u64g = (uint64_t)u32e + u32f;
    
    u16c = 0x9000U;
    u16d = 0x8000U;
    u32e = u16c + u16d; /* Processor dependent */
    
    u16c = 100U;
    s32 = 10 - (int16_t)u16c;
    
    if ((int32_t)u32e > -1) {
        u8a = 1U;
    }
    else
    {
        u8a = 0U;
    }
    
    u8a = 0xFFU;
    if ((uint8_t)~u8a == 0x00U)
    {
        u8b = 1U;
    }
    
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
        
        delay(1000000U / 2U);
    }
    
    // return 0;
}
