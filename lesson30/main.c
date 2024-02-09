#include "qpc.h"
#include "bsp.h"
#include "shape.h"
#include "rectangle.h"
#include <stdlib.h> /* For malloc() and free() */

Q_DEFINE_THIS_FILE

/* Function Prototypes */
void main_blinky1(QXThread *const me);
void main_blinky2(QXThread *const me);
void main_blinky3(QXThread *const me);

QXSemaphore SW1_sem;

uint32_t stack_blinky1[40] __attribute__((aligned(8)));
QXThread blinky1;
void main_blinky1(QXThread *const me)
{
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
    while (1)
    {
        BSP_sendMorseCode(0xE22A3800U); /* "TEST" */
        BSP_sendMorseCode(0xE22A3800U); /* "TEST" */
        QXThread_delay(5U);
    }
}

Shape s1; /* static allocation */
Rectangle r1;
uint32_t a;

int32_t main(void)
{
    Shape s2; /* automatic allocation */
    Shape *ps3 = malloc(sizeof(Shape)); /* dynamic allocation */
//    Shape const *ps1 = &s1;

    Shape_ctor(&s1, 1, 2);
    Shape_ctor(&s2, 3, 4);
    Shape_ctor(ps3, 5, 6);
    Rectangle_ctor(&r1, 1, 2, 15, 10);

    Shape_moveBy(&s1, 7, 8);
    Shape_moveBy(&s2, 9, 10);
    Shape_moveBy(ps3, -1, -2);
//    Shape_moveBy(ps1, -1, -2);
    Rectangle_draw(&r1);
    a = Rectangle_area(&r1);
    Shape_moveBy((Shape *)&r1, 7, 8);
    Q_ASSERT(Shape_distanceFrom((Shape *)&r1, (Shape *)&r1) == 0U);

    Q_ASSERT(Shape_distanceFrom(&s1, &s1) == 0U);
    Q_ASSERT(Shape_distanceFrom(&s1, &s2) == Shape_distanceFrom(&s2, &s1));
    Q_ASSERT(
            Shape_distanceFrom(&s1, &s2)
                    <= Shape_distanceFrom(&s1, ps3)
                            + Shape_distanceFrom(ps3, &s2));

    free(ps3);

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
