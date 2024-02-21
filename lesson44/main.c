#include "qpc.h"
#include "bsp.h"

typedef struct
{
    QEvt super;
    
    uint8_t ticks;
    uint32_t iter;
} BlinkPatternEvt;

typedef struct
{
    QActive super;
    
    QTimeEvt te;
    uint32_t iter;
} Blinky1;

QState Blinky1_initial(Blinky1 * const me, void const * const par);
QState Blinky1_active(Blinky1 * const me, QEvt const * const e);

void Blinky1_ctor(Blinky1 * const me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky1_initial));
    QTimeEvt_ctorX(&me->te, &me->super, TIMEOUT_SIG, 0U);
}

QState Blinky1_initial(Blinky1 * const me, void const * const par)
{
    QTimeEvt_armX(&me->te, 2U, 2U);
    me->iter = 2500U;
    return Q_TRAN(&Blinky1_active);
}

QState Blinky1_active(Blinky1 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case BLINK_PATTERN_SIG: {
            QTimeEvt_disarm(&me->te);
            QTimeEvt_armX(&me->te, ((BlinkPatternEvt const *)e)->ticks, 
                                   ((BlinkPatternEvt const *)e)->ticks);
            me->iter = ((BlinkPatternEvt const *)e)->iter;
            status_ = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            uint32_t volatile i = me->iter;
            for (; i != 0U; --i)
            {
                BSP_ledGreenOn();
                BSP_ledGreenOff();
            }
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

typedef struct
{
    QActive super;
    
    uint8_t seq;
} Blinky2;

QState Blinky2_initial(Blinky2 * const me, void const * const par);
QState Blinky2_active(Blinky2 * const me, QEvt const * const e);

void Blinky2_ctor(Blinky2 * const me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky2_initial));
}

QState Blinky2_initial(Blinky2 * const me, void const * const par) {
    me->seq = 0U;
    return Q_TRAN(&Blinky2_active);
}

QState Blinky2_active(Blinky2 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case BUTTON_PRESSED_SIG: {
            enum { N_SEQ = 2 };
            static uint8_t const n_ticks[N_SEQ] = {    2U,    1U };
            static uint32_t const n_iter[N_SEQ] = { 2500U, 1250U };
            
            me->seq = (me->seq + 1U) % N_SEQ;
            
            BlinkPatternEvt *bpe = Q_NEW(BlinkPatternEvt, BLINK_PATTERN_SIG);
            bpe->ticks = n_ticks[me->seq];
            for (uint32_t volatile i = 1250U; i != 0U; --i)
            {
                BSP_ledYellowOn();
                BSP_ledYellowOff();
            }
            bpe->iter = n_iter[me->seq];
            QACTIVE_POST(AO_Blinky1, &bpe->super, 0U);
            
            for (uint32_t volatile i = 1250U * 2U; i != 0U; --i)
            {
                BSP_ledYellowOn();
                BSP_ledYellowOff();
            }
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

QEvt const *blinky1_queue[10];
Blinky1 blinky1;

QEvt const *blinky2_queue[10];
Blinky2 blinky2;

static BlinkPatternEvt evtPool[10];

QActive * const AO_Blinky1 = &blinky1.super;
QActive * const AO_Blinky2 = &blinky2.super;

int32_t main(void)
{
    BSP_init();
    QF_init();
    
    QF_poolInit(evtPool, sizeof(evtPool), sizeof(evtPool[0]));

    Blinky1_ctor(&blinky1);
    QACTIVE_START(&blinky1,
                  5U, 
                  blinky1_queue, Q_DIM(blinky1_queue),
                  (void *)0, 0,
                  (void* )0);

    Blinky2_ctor(&blinky2);
    QACTIVE_START(&blinky2,
                  2U, 
                  blinky2_queue, Q_DIM(blinky2_queue),
                  (void *)0, 0,
                  (void *)0);

    /* Transfer control to the RTOS to run the threads */
    QF_run();

    return 0;
}
