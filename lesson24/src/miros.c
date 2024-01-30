#include "miros.h"
#include "qassert.h"
#include "stm32f10x.h"
#include "core_cm3.h"
#include <stdint.h>

Q_DEFINE_THIS_FILE

OSThread *volatile OS_curr; /* Pointer to current thread */
OSThread *volatile OS_next; /* Pointer to next thread to run */

OSThread *OS_thread[32U + 1U]; /* Array of threads started */
uint8_t OS_threadNum; /* Number of threads started so far */
uint8_t OS_currIdx; /* Current thread index for round robin scheduling */

void OS_init(void)
{
    /* Set the PendSV interrupt priority to the lowest level */
    *(uint32_t volatile*) 0xE000ED20 |= (0xFFU << 16U);
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

void OS_sched(void)
{
    /* OS_next = ... */
    ++OS_currIdx;
    if (OS_currIdx == OS_threadNum)
    {
        OS_currIdx = 0U;
    }
    OS_next = OS_thread[OS_currIdx];

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

    Q_ASSERT(OS_threadNum < Q_DIM(OS_thread));
    /* Register the thread with the OS */
    OS_thread[OS_threadNum] = me;
    ++OS_threadNum;
}
