#include "miros.h"
#include "qassert.h"
#include "stm32f10x.h"
#include "core_cm3.h"
#include <stdint.h>

Q_DEFINE_THIS_FILE

void main_idleThread(void);

OSThread *volatile OS_curr; /* Pointer to current thread */
OSThread *volatile OS_next; /* Pointer to next thread to run */

OSThread *OS_thread[32U + 1U]; /* Array of threads started */
uint32_t OS_readySet; /* Bitmask of threads that are ready to run */
uint32_t OS_delayedSet; /* Bitmask of threads that are in delay state */

#define LOG2(x) (32U - __CLZ(x))

OSThread idleThread;
void main_idleThread(void)
{
    while (1)
    {
        OS_onIdle();
    }
}

void OS_init(void *stackAry, uint32_t stackSize)
{
    /* Set the PendSV interrupt priority to the lowest level */
    *(uint32_t volatile*) 0xE000ED20 |= (0xFFU << 16U);

    /* Start idleThread thread */
    OSThread_start(&idleThread, 0U, main_idleThread, stackAry, stackSize);
}

void OS_run(void)
{
    /* Callback to configure and start interrupts */
    OS_onStartup();

    __disable_irq();
    OS_sched();
    __enable_irq();

    /* Should never reach here */
    Q_ERROR();
}

void OS_tick(void)
{
    uint32_t bit;

    uint32_t workingSet = OS_delayedSet;
    while (workingSet != 0U)
    {
        OSThread *t = OS_thread[LOG2(workingSet)];
        Q_ASSERT((t != (OSThread* )0) && (t->timeout != 0U));

        --t->timeout;
        bit = (1U << (t->priority - 1U));
        if (t->timeout == 0U)
        {
            OS_readySet |= bit;
            OS_delayedSet &= ~bit;
        }

        workingSet &= ~bit;
    }
}

void OS_delay(uint32_t ticks)
{
    uint32_t bit;

    __disable_irq();

    /* Never call OS_delay from the idleThread */
    Q_REQUIRE(OS_curr != OS_thread[0]);

    OS_curr->timeout = ticks;
    bit = (1U << (OS_curr->priority - 1U));
    OS_readySet &= ~bit;
    OS_delayedSet |= bit;
    OS_sched();
    __enable_irq();
}

void OS_sched(void)
{
    /* OS_next = ... */
    if (OS_readySet == 0U) /* Idle condition? */
    {
        OS_next = OS_thread[0]; /* Idle thread */
    }
    else
    {
        OS_next = OS_thread[LOG2(OS_readySet)];
        Q_ASSERT(OS_next != (OSThread* )0);
    }

    if (OS_next != OS_curr)
    {
        *(uint32_t volatile*) 0xE000ED04 |= (1U << 28U);
    }
}

void OSThread_start(OSThread *me, uint8_t priority,
        OSThreadHandler threadHandler, void *stackAry, uint32_t stackSize)
{
    /* Make sure stack memory boundary is 8-byte aligned */
    uint32_t *sp = (uint32_t*) ((uint32_t) (((uint64_t) ((uint32_t) stackAry
            + stackSize) * 8U) / 8U));
    uint32_t *sp_limit;

    /* Priority must be in range and unused */
    Q_REQUIRE(
            (priority < Q_DIM(OS_thread)) && (OS_thread[priority] == (OSThread *)0));

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

    /* Register the thread with the OS */
    OS_thread[priority] = me;
    me->priority = priority;
    /* Make the thread ready to run */
    if (priority > 0U)
    {
        OS_readySet |= (1U << (priority - 1U));
    }
}

void PendSV_Handler(void);
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
