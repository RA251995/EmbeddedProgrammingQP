/* TimeBomb/Button with uC/OS-II RTOS */
#include "qassert.h"  /* Embedded-system-friendly assertions */
#include "bsp.h"      /* Board Support Package */
#include <stdbool.h>

Q_DEFINE_THIS_MODULE("main") /* this module name for Q_ASSERT() */

/* The TimeBomb AO =============================================================*/
typedef struct
{
    Active super; /* Inherit Active base class */
    /* Private data for the AO */
    enum 
    { 
        WAIT4BUTTON_STATE,
        BLINK_STATE,
        PAUSE_STATE,
        BOOM_STATE
    } state; /* The "state" variable */
    
    TimeEvent te;
    INT32U blink_ctr;
} TimeBomb;

static void TimeBomb_dispatch(TimeBomb * const me, Event const * const e)
{
    if (e->sig == INIT_SIG)
    {
        BSP_ledGreenOn();
        me->state = WAIT4BUTTON_STATE;
        return;
    }
    
    switch(me->state)
    {
        case WAIT4BUTTON_STATE:
        {
            switch(e->sig)
            {
                case BUTTON_PRESSED_SIG:
                {
                    BSP_ledGreenOff();
                    BSP_ledOrangeOn();
                    TimeEvent_arm(&me->te, OS_TICKS_PER_SEC / 2U, 0U);
                    me->blink_ctr = 10U;
                    me->state = BLINK_STATE;
                    break;
                }
            }
            break;
        }
        case BLINK_STATE:
        {
            switch(e->sig)
            {
                case TIMEOUT_SIG:
                {
                    BSP_ledOrangeOff();
                    TimeEvent_arm(&me->te, OS_TICKS_PER_SEC / 2U, 0U);
                    me->state = PAUSE_STATE;
                    break;
                }
            }
            break;
        }
        case PAUSE_STATE:
        {
            switch(e->sig)
            {
                case TIMEOUT_SIG:
                {
                    --me->blink_ctr;
                    if (me->blink_ctr > 0U)
                    {
                        BSP_ledOrangeOn();
                        TimeEvent_arm(&me->te, OS_TICKS_PER_SEC / 2U, 0U);
                        me->state = BLINK_STATE;
                    }
                    else
                    {
                        BSP_ledGreenOn();
                        BSP_ledYellowOn();
                        BSP_ledOrangeOn();
                        me->state = BOOM_STATE; 
                    }
                    break;
                }
            }
            break;
        }
        case BOOM_STATE:
        {
            break;
        }
        default:
        {
            Q_ERROR(); /* Invalid state */
            break;
        }
    }
}

void TimeBomb_ctor(TimeBomb * const me)
{
    Active_ctor(&me->super, (DispatchHandler)&TimeBomb_dispatch);
    TimeEvent_ctor(&me->te, TIMEOUT_SIG, &me->super);
}

OS_STK timeBomb_stack[100]; /* task stack */
static Event *timeBomb_queue[10];
static TimeBomb timeBomb;
Active *AO_TimeBomb = &timeBomb.super;

/* the main function =========================================================*/
int main() {
    INT8U err;

    BSP_init(); /* initialize the BSP */
    OSInit();   /* initialize uC/OS-II */

    /* create AOs and start it */
    TimeBomb_ctor(&timeBomb);
    Active_start(AO_TimeBomb,
                 2U,
                 timeBomb_queue,
                 sizeof(timeBomb_queue)/sizeof(timeBomb_queue[0]),
                 timeBomb_stack,
                 sizeof(timeBomb_stack),
                 0U);

    BSP_start(); /* configure and start the interrupts */

    OSStart(); /* start the uC/OS-II scheduler... */
    return 0; /* NOTE: the scheduler does NOT return */
}

/*******************************************************************************
* NOTE1:
* The call to uC/OS-II API OSTaskCreateExt() assumes that the pointer to the
* top-of-stack (ptos) is at the end of the provided stack memory. This is
* correct only for CPUs with downward-growing stack, but must be changed for
* CPUs with upward-growing stack.
*/
