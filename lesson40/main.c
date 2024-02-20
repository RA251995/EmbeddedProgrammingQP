/* TimeBomb/Button with uC/OS-II RTOS */
#include "bsp.h"      /* Board Support Package */
#include <stdbool.h>

Q_DEFINE_THIS_MODULE("main") /* this module name for Q_ASSERT() */

/* The TimeBomb AO =============================================================*/
typedef struct
{
    QActive Q_SUPER; /* Inherit Active base class */
    /* Private data for the AO */
    QTimeEvt te;
    uint32_t blink_ctr;
} TimeBomb;

QState TimeBomb_initial(TimeBomb * const me, QEvt const * const e);
QState TimeBomb_armed(TimeBomb * const me, QEvt const * const e);
QState TimeBomb_defused(TimeBomb * const me, QEvt const * const e);
QState TimeBomb_wait4button(TimeBomb * const me, QEvt const * const e);
QState TimeBomb_blink(TimeBomb * const me, QEvt const * const e);
QState TimeBomb_pause(TimeBomb * const me, QEvt const * const e);
QState TimeBomb_boom(TimeBomb * const me, QEvt const * const e);

QState TimeBomb_initial(TimeBomb * const me, QEvt const * const e)
{
    return Q_TRAN(TimeBomb_wait4button);
}

QState TimeBomb_armed(TimeBomb * const me, QEvt const * const e)
{
    QState status;
    
    switch(e->sig)
    {
        case Q_EXIT_SIG:
        {
            BSP_ledGreenOff();
            BSP_ledYellowOff();
            BSP_ledOrangeOff();
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG:
        {
            status = Q_TRAN(TimeBomb_wait4button);
            break;
        }
        case BUTTON2_PRESSED_SIG:
        {
            status = Q_TRAN(TimeBomb_defused);
            break;
        }
        default:
        {
            status = Q_SUPER(QHsm_top);
            break;
        }
    }
    
    return status;
}

QState TimeBomb_defused(TimeBomb * const me, QEvt const * const e)
{
    QState status;
    
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            BSP_ledYellowOn();
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG:
        {
            BSP_ledYellowOff();
            status = Q_HANDLED();
            break;
        }
        case BUTTON2_PRESSED_SIG:
        {
            status = Q_TRAN(TimeBomb_armed);
            break;
        }
        default:
        {
            status = Q_SUPER(QHsm_top);
            break;
        }
    }
    
    return status;
}

QState TimeBomb_wait4button(TimeBomb * const me, QEvt const * const e)
{
    QState status;
    
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            BSP_ledGreenOn();
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG:
        {
            BSP_ledGreenOff();
            status = Q_HANDLED();
            break;
        }
        case BUTTON_PRESSED_SIG:
        {        
            me->blink_ctr = 5U;
            status = Q_TRAN(TimeBomb_blink);
            break;
        }
        default:
        {
            status = Q_SUPER(TimeBomb_armed);
            break;
        }
    }
    
    return status;
}

QState TimeBomb_blink(TimeBomb * const me, QEvt const * const e)
{
    QState status;
    
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            BSP_ledOrangeOn();
            QTimeEvt_armX(&me->te, BSP_TICKS_PER_SEC / 2U, 0U);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG:
        {
            BSP_ledOrangeOff();
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG:
        {
            status = Q_TRAN(TimeBomb_pause);
            break;
        }
        default:
        {
            status = Q_SUPER(TimeBomb_armed);
            break;
        }
    }
    
    return status;
}

QState TimeBomb_pause(TimeBomb * const me, QEvt const * const e)
{
    QState status;
    
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            QTimeEvt_armX(&me->te, BSP_TICKS_PER_SEC / 2U, 0U);
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG:
        {
            --me->blink_ctr;
            if (me->blink_ctr > 0U)
            {
                status = Q_TRAN(TimeBomb_blink);
            }
            else
            {
                status = Q_TRAN(TimeBomb_boom);
            }
            break;
        }
        default:
        {
            status = Q_SUPER(TimeBomb_armed);
            break;
        }
    }
    
    return status;
}

QState TimeBomb_boom(TimeBomb * const me, QEvt const * const e)
{
    QState status;
    
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            BSP_ledGreenOn();
            BSP_ledYellowOn();
            BSP_ledOrangeOn();
            status = Q_HANDLED();
            break;
        }
        default:
        {
            status = Q_SUPER(TimeBomb_armed);
            break;
        }
    }
    
    return status;
}

void TimeBomb_ctor(TimeBomb * const me)
{
    QActive_ctor(&me->Q_SUPER, (QStateHandler)&TimeBomb_initial);
    QTimeEvt_ctorX(&me->te, &me->Q_SUPER, TIMEOUT_SIG, 0U);
}

static QEvt *timeBomb_queue[10];
static TimeBomb timeBomb;
QActive *AO_TimeBomb = &timeBomb.Q_SUPER;

/* the main function =========================================================*/
int main() {
    BSP_init(); /* initialize the BSP */
    QF_init();

    /* create AOs and start it */
    TimeBomb_ctor(&timeBomb);
    QACTIVE_START(AO_TimeBomb,
                  2U,
                  (const QEvt **)timeBomb_queue,
                  sizeof(timeBomb_queue)/sizeof(timeBomb_queue[0]),
                  (void *)0, 0U,
                  (void *)0);

    QF_run();
    return 0; /* NOTE: the scheduler does NOT return */
}

/*******************************************************************************
* NOTE1:
* The call to uC/OS-II API OSTaskCreateExt() assumes that the pointer to the
* top-of-stack (ptos) is at the end of the provided stack memory. This is
* correct only for CPUs with downward-growing stack, but must be changed for
* CPUs with upward-growing stack.
*/
