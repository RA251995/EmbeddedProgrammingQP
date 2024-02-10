#include "qpc.h"
#include "bsp.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include <stdlib.h> /* For malloc() and free() */

Q_DEFINE_THIS_FILE

/* Function Prototypes */
void main_blinky1(QXThread *const me);
void main_blinky2(QXThread *const me);
void main_blinky3(QXThread *const me);

QXSemaphore SW1_sem;

Shape s1(1, 2); /* static allocation */
Rectangle r1(1, 2, 15, 10);
Circle c1(3, 4, 10);
uint32_t a;

uint32_t stack_blinky1[40] __attribute__((aligned(8)));
QXThread blinky1;
void main_blinky1(QXThread *const me)
{
    while (1)
    {
        BSP_sendMorseCode(0xA8EEE2A0U); /* "SOS" */
        
        s1.moveBy(7, 8);
        
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
            
            Q_ASSERT(s1.distanceFrom(&s1) == 0U);
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

int32_t main(void)
{
    Shape s2(3, 4); /* automatic allocation */
    Shape *ps3 = new Shape(5, 6); /* dynamic allocation */
    
    Shape *ps = &r1; /* upcasting */
    
    r1.draw(); /* early binding */
    ps->draw(); /* late binding */
    a = ps->area();
    
    Shape const *graph[] = {
        &c1,
        &r1,
        ps3,
        (Shape *)0
    };
    drawGraph(graph);

    s1.moveBy(7, 8);
    s2.moveBy(9, 10);
    ps3->moveBy(-1, -2);
    
    r1.draw();
    a = r1.area();
    r1.moveBy(7, 8);
    Q_ASSERT(r1.distanceFrom(&r1) == 0U);

    Q_ASSERT(s1.distanceFrom(&s1) == 0U);
    Q_ASSERT(s1.distanceFrom(&s2) == s2.distanceFrom( &s1));
    Q_ASSERT(
            s1.distanceFrom(&s2)
                    <= s1.distanceFrom(ps3)
                            + ps3->distanceFrom(&s2));

    delete ps3;

    QF_init();
    BSP_init();

    QXSemaphore_init(&SW1_sem, 0U, 1U);

    QXThread_ctor(&blinky1, &main_blinky1, 0U);
    QXTHREAD_START(&blinky1, 5U, (const QEvt **)0, 0, &stack_blinky1,
            sizeof(stack_blinky1), (void* )0);

    QXThread_ctor(&blinky2, &main_blinky2, 0U);
    QXTHREAD_START(&blinky2, 2U, (const QEvt **)0, 0, &stack_blinky2,
            sizeof(stack_blinky2), (void* )0);

    QXThread_ctor(&blinky3, &main_blinky3, 0U);
    QXTHREAD_START(&blinky3, 1U, (const QEvt **)0, 0, &stack_blinky3,
            sizeof(stack_blinky3), (void* )0);

    /* Transfer control to the RTOS to run the threads */
    QF_run();

    return 0;
}
