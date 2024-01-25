/* Includes */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"
#include <stdint.h>

/* Constants */
#define SYSCLK_FREQ_72MHz 72000000

/* Externs */
extern int32_t main(void);

/* Variables */
uint32_t SystemCoreClock = SYSCLK_FREQ_72MHz;

/* Function Prototypes */
void __NO_RETURN __attribute__ ((naked)) _start(void);
void __NO_RETURN __attribute__ ((naked)) Reset_Handler(void);
static void SetSysClock(void);
static void SetSysClockTo72(void);

/* Global Functions */
void __NO_RETURN __attribute__ ((naked)) Reset_Handler(void)
{
    __PROGRAM_START();
}

void __NO_RETURN _start(void)
{
    SystemInit();
    main();

    __ASM volatile("b .");
}

void SystemInit(void)
{
    /* Reset the RCC clock configuration to the default reset state (for debug purpose) */
    /* Set HSION bit */
    RCC->CR |= (uint32_t) 0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t) 0xF8FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t) 0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t) 0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE bits */
    RCC->CFGR &= (uint32_t) 0xFF80FFFF;

    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x009F0000;

    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    SetSysClock();
}

/* Local Functions */
static void SetSysClock(void)
{
#if defined SYSCLK_FREQ_72MHz
    SetSysClockTo72();
#endif

    /* If none of the define above is enabled, the HSI is used as System clock
     source (default after reset) */
}

static void SetSysClockTo72(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CR |= ((uint32_t) RCC_CR_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t) 0x01;
    }
    else
    {
        HSEStatus = (uint32_t) 0x00;
    }

    if (HSEStatus == (uint32_t) 0x01)
    {
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        /* Flash 2 wait state */
        FLASH->ACR &= (uint32_t) ((uint32_t) ~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t) FLASH_ACR_LATENCY_2;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE1_DIV2;

        /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
        RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_PLLSRC
                | RCC_CFGR_PLLXTPRE |
                RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* Wait till PLL is ready */
        while ((RCC->CR & RCC_CR_PLLRDY) == 0)
        {
        }

        /* Select PLL as system clock source */
        RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t) RCC_CFGR_SW_PLL;

        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != (uint32_t) 0x08)
        {
        }
    }
    else
    {
        /* If HSE fails to start-up, the application will have wrong clock
         * configuration */
        assert_failed(__FILE__, __LINE__);
    }
}
