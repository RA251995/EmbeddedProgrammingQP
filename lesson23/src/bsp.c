/* Board Support Package */
/* Includes */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "bsp.h"
#include "miros.h"

/* Externs */
extern OSThread *volatile OS_curr;
extern OSThread *volatile OS_next;

/* Function Prototypes */
void __NO_RETURN __attribute__ ((naked)) NMI_Handler(void);
void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void);
void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void);
void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void);
void SysTick_Handler(void);
void PendSV_Handler(void);

/* Variable Declarations */
static uint32_t volatile l_tickCtr;

/* Global Functions */
void SysTick_Handler(void)
{
    ++l_tickCtr;

    __disable_irq();
    OS_sched();
    __enable_irq();
}

void __attribute__((naked)) PendSV_Handler(void)
{
    __ASM volatile (
            /* __disable_irq(); */
            "CPSID    I\n"

            /* if (OS_curr != (OSThread *)0) { */
            "LDR      r0,=OS_curr\n"
            "LDR      r0,[r0,#0x00]\n"
            "CBZ      r0,PendSV_restore\n"

            /* Push registers R4 - R11 */
            "PUSH     {r4-r11}\n"

            /* OS_curr->sp = sp; */
            "LDR      r1,=OS_curr\n"
            "LDR      r1,[r1,#0x00]\n"
            "STR      sp,[r1,#0x00]\n"
            /* } */

            "PendSV_restore:\n"
            /* sp = OS_next->sp; */
            "LDR      r0,=OS_next\n"
            "LDR      r1,[r0,#0x00]\n"
            "LDR      sp,[r1,#0x00]\n"

            /* OS_curr = OS_next; */
            "LDR      r0,=OS_next\n"
            "LDR      r0,[r0,#0x00]\n"
            "LDR      r1,=OS_curr\n"
            "STR      r0,[r1,#0x00]\n"

            /* Pop registers R4-R11 */
            "POP      {r4-r11}\n"

            /* __enable_irq(); */
            "CPSIE    I\n"

            "BX lr"
    );
}

void BSP_init(void)
{
    RCC->APB2ENR |= (1U << 2U); /* Enable GPIOA clock */
    GPIOA->CRL |= (0x1U | (0x1U << 4U) | (0x1U << 8U)); /* Set PA0..2 mode to output */
    GPIOA->CRL &= ~((0x3U << 2U) | (0x3U << 6U) | (0x3U << 10U)); /* Set PA0..2 configuration to push-pull */

    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); /* TODO: Assert return */

    /* Set SysTick interrupt priority (highest) */
    NVIC_SetPriority(SysTick_IRQn, 0U);

    __enable_irq();
}

uint32_t BSP_tickCtr(void)
{
    uint32_t tickCtr;

    __disable_irq();
    tickCtr = l_tickCtr;
    __enable_irq();

    return tickCtr;
}

void BSP_delay(uint32_t ticks)
{
    uint32_t start = BSP_tickCtr(); /* Not portable */
    while ((BSP_tickCtr() - start) < ticks)
    {
    }
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

void assert_failed(const char *module, int32_t id)
{
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
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) HardFault_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) MemManage_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) BusFault_Handler(void)
{
    NVIC_SystemReset();
}

void __NO_RETURN __attribute__ ((naked)) UsageFault_Handler(void)
{
    NVIC_SystemReset();
}
