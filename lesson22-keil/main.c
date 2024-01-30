/* Includes */
#include "bsp.h"
#include <stdint.h>

/* Function Prototypes */
void main_blinky1(void);
void main_blinky2(void);

uint32_t stack_blinky1[40] __attribute__((aligned(8)));
uint32_t *sp_blinky1 = &stack_blinky1[40];
void main_blinky1(void)
{
    while (1)
    {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40] __attribute__((aligned(8)));
uint32_t *sp_blinky2 = &stack_blinky2[40];
void main_blinky2(void)
{
    while (1)
    {
        BSP_ledOrangeOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledOrangeOff();
        BSP_delay(BSP_TICKS_PER_SEC / 3U);
    }
}

int32_t main(void)
{
    BSP_init();

    /* Fabricate Cortex-M ISR stack frame for main_blinky1 */
    *(--sp_blinky1) = (1U << 24U); /* xPSR */
    *(--sp_blinky1) = (uint32_t) &main_blinky1; /* PC */
    *(--sp_blinky1) = 0x0E; /* LR */
    *(--sp_blinky1) = 0x0C; /* R12 */
    *(--sp_blinky1) = 0x03; /* R3 */
    *(--sp_blinky1) = 0x02; /* R2 */
    *(--sp_blinky1) = 0x01; /* R1 */
    *(--sp_blinky1) = 0x00; /* R0 */

    /* Fabricate Cortex-M ISR stack frame for main_blinky2 */
    *(--sp_blinky2) = (1U << 24U); /* xPSR */
    *(--sp_blinky2) = (uint32_t) &main_blinky2; /* PC */
    *(--sp_blinky2) = 0x0E; /* LR */
    *(--sp_blinky2) = 0x0C; /* R12 */
    *(--sp_blinky2) = 0x03; /* R3 */
    *(--sp_blinky2) = 0x02; /* R2 */
    *(--sp_blinky2) = 0x01; /* R1 */
    *(--sp_blinky2) = 0x00; /* R0 */

    while(1)
    {
    }

    // return 0;
}
