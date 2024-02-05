#include "qpc.h"
#include "bsp.h"
#include "shape.h"
#include <stdlib.h> /* For malloc() and free() */

Q_DEFINE_THIS_FILE

/* Function Prototypes */
void main_blinky1(QXThread *const me);
void main_blinky2(QXThread *const me);
void main_blinky3(QXThread *const me);

QXSemaphore SW1_sem;

static uint32_t stack_blinky1[40];
static QXThread blinky1;
void main_blinky1(QXThread * const me) {
    (void) me; /* unused parameter */
    
    while (1) {
        BSP_sendMorseCode(0xA8EEE2A0U); /* "SOS" */
        QXThread_delay(1U); /* block for 1 tick */
    }
}

static uint32_t stack_blinky2[40];
static QXThread blinky2;
void main_blinky2(QXThread * const me) {
    (void) me; /* unused parameter */
    
    while (1) {
        uint32_t volatile i;

        QXSemaphore_wait(&SW1_sem,  /* pointer to semaphore to wait on */
                         QXTHREAD_NO_TIMEOUT); /* timeout for waiting */

        for (i = 3*1500U; i != 0U; --i) {
            //BSP_ledYellowOn();
            //BSP_ledYellowOff();
            BSP_ledYellowToggle();
        }
    }
}

static uint32_t stack_blinky3[40];
static QXThread blinky3;
void main_blinky3(QXThread * const me) {
    (void) me; /* unused parameter */
    
    while (1) {
        BSP_sendMorseCode(0xE22A3800U); /* "TEST" */
        BSP_sendMorseCode(0xE22A3800U); /* "TEST" */
        QXThread_delay(5U);
    }
}

static Shape s1; /* static allocation */

int main(void) {
    Shape s2; /* automatic allocation */
    Shape *ps3 = (Shape *)malloc(sizeof(Shape)); /* dynamic allocation */
//    Shape const *ps1 = &s1;

    Shape_ctor(&s1, 1, 2);
    Shape_ctor(&s2, 3, 4);
    Shape_ctor(ps3, 5, 6);

    Shape_moveBy(&s1, 7, 8);
    Shape_moveBy(&s2, 9, 10);
    Shape_moveBy(ps3, -1, -2);
//    Shape_moveBy(ps1, -1, -2);

    Q_ASSERT(Shape_distanceFrom(&s1, &s1) == 0U);
    Q_ASSERT(Shape_distanceFrom(&s1, &s2) == Shape_distanceFrom(&s2, &s1));
    Q_ASSERT(
            Shape_distanceFrom(&s1, &s2)
                    <= Shape_distanceFrom(&s1, ps3)
                            + Shape_distanceFrom(ps3, &s2));

    free(ps3);
    
    QF_init();
    BSP_init();

    /* initialize the SW1_sema semaphore as binary, signaling semaphore */
    QXSemaphore_init(&SW1_sem, /* pointer to semaphore to initialize */
                     0U,  /* initial semaphore count (singaling semaphore) */
                     1U); /* maximum semaphore count (binary semaphore) */

    /* initialize and start blinky1 thread */
    QXThread_ctor(&blinky1, &main_blinky1, 0);
    QXTHREAD_START(&blinky1,
                   5U, /* priority */
                   (const QEvt **)0, 0, /* message queue (not used) */
                   stack_blinky1, sizeof(stack_blinky1), /* stack */
                   (void *)0); /* extra parameter (not used) */

    /* initialize and start blinky2 thread */
    QXThread_ctor(&blinky2, &main_blinky2, 0);
    QXTHREAD_START(&blinky2,
                   2U, /* priority */
                   (const QEvt **)0, 0, /* message queue (not used) */
                   stack_blinky2, sizeof(stack_blinky2), /* stack */
                   (void *)0); /* extra parameter (not used) */

    /* initialize and start blinky3 thread */
    QXThread_ctor(&blinky3, &main_blinky3, 0);
    QXTHREAD_START(&blinky3,
                   1U, /* priority */
                   (const QEvt **)0, 0, /* message queue (not used) */
                   stack_blinky3, sizeof(stack_blinky3), /* stack */
                   (void *)0); /* extra parameter (not used) */

    /* transfer control to the RTOS to run the threads */
    return QF_run();
}
