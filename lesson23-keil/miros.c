#include "miros.h"
#include <stdint.h>

OSThread * volatile OS_curr; /* Pointer to current thread */
OSThread * volatile OS_next; /* Pointer to next thread to run */

void OS_init(void)
{
    /* Set the PendSV interrupt priority to the lowest level */
    *(uint32_t volatile*) 0xE000ED20 |= (0xFFU << 16U);
}

void OS_sched(void)
{
    /* OS_next = ... */
    if (OS_next != OS_curr)
    {
        *(uint32_t volatile*) 0xE000ED04 |= (1U << 28U);
    }
}

void OSThread_start(OSThread *me, OSThreadHandler threadHandler, void *stackAry,
        uint32_t stackSize)
{
    /* Make sure stack memory boundary is 8-byte aligned */
    uint32_t *sp = (uint32_t*) ((uint32_t) (((uint64_t) ((uint32_t) stackAry
            + stackSize) * 8U) / 8U));
    uint32_t *sp_limit;

    /* Fabricate Cortex-M ISR stack frame for main_blinky1 */
    *(--sp) = (1U << 24U); /* xPSR */
    *(--sp) = (uint32_t) threadHandler; /* PC */
    *(--sp) = 0x0000000E; /* LR */
    *(--sp) = 0x0000000C; /* R12 */
    *(--sp) = 0x00000003; /* R3 */
    *(--sp) = 0x00000002; /* R2 */
    *(--sp) = 0x00000001; /* R1 */
    *(--sp) = 0x00000000; /* R0 */
    /* Additionally, fake registers R4-R11 */
    *(--sp) = 0x0000000B; /* R11 */
    *(--sp) = 0x0000000A; /* R10 */
    *(--sp) = 0x00000009; /* R9 */
    *(--sp) = 0x00000008; /* R8 */
    *(--sp) = 0x00000007; /* R7 */
    *(--sp) = 0x00000006; /* R6 */
    *(--sp) = 0x00000005; /* R5 */
    *(--sp) = 0x00000004; /* R4 */

    me->sp = (void*) sp;

    sp_limit = (uint32_t*) (((((uint32_t) stackAry - 1U) / 8U) + 1U) * 8U);
    /* Fill stack with known value */
    for (sp = sp - 1; sp >= sp_limit; --sp)
    {
        *sp = 0xDEADBEEFU;
    }
}

__asm
void PendSV_Handler(void)
{
    IMPORT OS_curr /* extern variable */
    IMPORT OS_next /* extern variable */
    
    /* __disable_irq(); */
    CPSID    I
    
    /* if (OS_curr != (OSThread *)0) { */ 
    LDR      r1,=OS_curr
    LDR      r1,[r1,#0x00]
    CBZ      r1,PendSV_restore
    
    /*      Push registers r4-r11 on the stack */ 
    PUSH     {r4-r11}
    
    /*      OS_curr->sp = sp; */
    STR      sp,[r1,#0x00]
    /* } */ 

PendSV_restore
    /* sp = OS_next->sp; */
    LDR      r1,=OS_next
    LDR      r1,[r1,#0x00]
    LDR      sp,[r1,#0x00]

    /* OS_curr = OS_next; */
    LDR      r1,=OS_next
    LDR      r1,[r1,#0x00]
    LDR      r2,=OS_curr
    STR      r1,[r2,#0x00]

    /* Pop registers r4-r11 */
    POP {r4-r11}

    /* __enable_irq(); */ 
    CPSIE    I
    
    /* Return to next thread */
    BX       lr
}
