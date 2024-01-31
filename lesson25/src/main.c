/* Includes */
#include "bsp.h"
#include "miros.h"
#include <stdint.h>

/* Function Prototypes */
void main_blinky1(void);
void main_blinky2(void);
void main_blinky3(void);

uint32_t stack_blinky1[40] __attribute__((aligned(8)));
OSThread blinky1;
void main_blinky1(void)
{
    while (1)
    {
        BSP_ledGreenOn();
        OS_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        OS_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40] __attribute__((aligned(8)));
OSThread blinky2;
void main_blinky2(void)
{
    while (1)
    {
        BSP_ledOrangeOn();
        OS_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledOrangeOff();
        OS_delay(BSP_TICKS_PER_SEC / 3U);
    }
}

uint32_t stack_blinky3[40] __attribute__((aligned(8)));
OSThread blinky3;
void main_blinky3(void)
{
    while (1)
    {
        BSP_ledYellowOn();
        OS_delay(BSP_TICKS_PER_SEC / 3U);
        BSP_ledYellowOff();
        OS_delay(BSP_TICKS_PER_SEC * 3U / 5U);
    }
}

uint32_t stackIdleThread[40] __attribute__((aligned(8)));

int32_t main(void)
{
    BSP_init();
    OS_init(stackIdleThread, sizeof(stackIdleThread));

    OSThread_start(&blinky1, &main_blinky1, &stack_blinky1, sizeof(stack_blinky1));
    OSThread_start(&blinky2, &main_blinky2, &stack_blinky2, sizeof(stack_blinky2));
    OSThread_start(&blinky3, &main_blinky3, &stack_blinky3, sizeof(stack_blinky3));

    /* Transfer control to the RTOS to run the threads */
    OS_run();

    return 0;
}
