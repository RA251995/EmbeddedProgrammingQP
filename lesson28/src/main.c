/* Includes */
#include "qpc.h"
#include "bsp.h"

/* Function Prototypes */
void main_blinky1(QXThread *const me);
void main_blinky2(QXThread *const me);
void main_blinky3(QXThread *const me);

QXSemaphore SW1_sem;

uint32_t stack_blinky1[40] __attribute__((aligned(8)));
QXThread blinky1;
void main_blinky1(QXThread *const me)
{
    (void) me; /* Unused parameter */

    while (1)
    {
        BSP_sendMorseCode(0xA8EEE2A0U); /* "SOS" */
        QXThread_delay(1U);
    }
}

uint32_t stack_blinky2[40] __attribute__((aligned(8)));
QXThread blinky2;
void main_blinky2(QXThread *const me)
{
    (void) me; /* Unused parameter */

    while (1)
    {
        uint32_t volatile i;

        QXSemaphore_wait(&SW1_sem, QXTHREAD_NO_TIMEOUT);

        for (i = 1250U * 3U; i != 0U; --i)
        {
//            BSP_ledYellowOn();
//            BSP_ledYellowOff();
            BSP_ledYellowToggle();
        }
    }
}

uint32_t stack_blinky3[40] __attribute__((aligned(8)));
QXThread blinky3;
void main_blinky3(QXThread *const me)
{
    (void) me; /* Unused parameter */

    while (1)
    {
        BSP_sendMorseCode(0xE22A3800U); /* "TEST" */
        BSP_sendMorseCode(0xE22A3800U); /* "TEST" */
        QXThread_delay(5U);
    }
}

int32_t main(void)
{
    QF_init();
    BSP_init();

    QXSemaphore_init(&SW1_sem, 0U, 1U);

    QXThread_ctor(&blinky1, &main_blinky1, 0U);
    QXTHREAD_START(&blinky1, 5U, (void* )0, 0, &stack_blinky1,
            sizeof(stack_blinky1), (void* )0);

    QXThread_ctor(&blinky2, &main_blinky2, 0U);
    QXTHREAD_START(&blinky2, 2U, (void* )0, 0, &stack_blinky2,
            sizeof(stack_blinky2), (void* )0);

    QXThread_ctor(&blinky3, &main_blinky3, 0U);
    QXTHREAD_START(&blinky3, 1U, (void* )0, 0, &stack_blinky3,
            sizeof(stack_blinky3), (void* )0);

    /* Transfer control to the RTOS to run the threads */
    QF_run();

    return 0;
}
