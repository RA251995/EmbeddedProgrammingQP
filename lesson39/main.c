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
    TimeEvent te;
    INT32U blink_ctr;
} TimeBomb;

State TimeBomb_initial(TimeBomb * const me, Event const * const e);
State TimeBomb_wait4button(TimeBomb * const me, Event const * const e);
State TimeBomb_blink(TimeBomb * const me, Event const * const e);
State TimeBomb_pause(TimeBomb * const me, Event const * const e);
State TimeBomb_boom(TimeBomb * const me, Event const * const e);

State TimeBomb_initial(TimeBomb * const me, Event const * const e)
{
    return TRAN(TimeBomb_wait4button);
}

State TimeBomb_wait4button(TimeBomb * const me, Event const * const e)
{
    State status;
    
    switch(e->sig)
    {
        case ENTRY_SIG:
        {
            BSP_ledGreenOn();
            status = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG:
        {
            BSP_ledGreenOff();
            status = HANDLED_STATUS;
            break;
        }
        case BUTTON_PRESSED_SIG:
        {        
            me->blink_ctr = 5U;
            status = TRAN(TimeBomb_blink);
            break;
        }
        default:
        {
            status = IGNORED_STATUS;
            break;
        }
    }
    
    return status;
}

State TimeBomb_blink(TimeBomb * const me, Event const * const e)
{
    State status;
    
    switch(e->sig)
    {
        case ENTRY_SIG:
        {
            BSP_ledOrangeOn();
            TimeEvent_arm(&me->te, OS_TICKS_PER_SEC / 2U, 0U);
            status = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG:
        {
            BSP_ledOrangeOff();
            status = HANDLED_STATUS;
            break;
        }
        case TIMEOUT_SIG:
        {
            status = TRAN(TimeBomb_pause);
            break;
        }
        default:
        {
            status = IGNORED_STATUS;
            break;
        }
    }
    
    return status;
}

State TimeBomb_pause(TimeBomb * const me, Event const * const e)
{
    State status;
    
    switch(e->sig)
    {
        case ENTRY_SIG:
        {
            TimeEvent_arm(&me->te, OS_TICKS_PER_SEC / 2U, 0U);
            status = HANDLED_STATUS;
            break;
        }
        case TIMEOUT_SIG:
        {
            --me->blink_ctr;
            if (me->blink_ctr > 0U)
            {
                status = TRAN(TimeBomb_blink);
            }
            else
            {
                status = TRAN(TimeBomb_boom);
            }
            break;
        }
        default:
        {
            status = IGNORED_STATUS;
            break;
        }
    }
    
    return status;
}

State TimeBomb_boom(TimeBomb * const me, Event const * const e)
{
    State status;
    
    switch(e->sig)
    {
        case ENTRY_SIG:
        {
            BSP_ledGreenOn();
            BSP_ledYellowOn();
            BSP_ledOrangeOn();
            status = HANDLED_STATUS;
            break;
        }
        default:
        {
            status = IGNORED_STATUS;
            break;
        }
    }
    
    return status;
}

void TimeBomb_ctor(TimeBomb * const me)
{
    Active_ctor(&me->super, (StateHandler)&TimeBomb_initial);
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
