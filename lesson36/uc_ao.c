/*****************************************************************************
* Active Object pattern implementation based on uC/OS-II (uC/AO)
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2020 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: APACHE-2.0
*
* This software is distributed by Quantum Leaps, LLC under the terms of
* Apache License Version 2.0, which is the same license used for uC/OS-II RTOS.
* The text of the license is available at: www.apache.org/licenses/LICENSE-2.0.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
*****************************************************************************/
#include "uc_ao.h" /* uC/AO interface */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("uc_ao") /* Module name for Q_ASSERT */

void Active_ctor(Active * const me, DispatchHandler dispatch)
{
    me->dispatch = dispatch; /* attach the dispatch handler for the "me" AO */
}

/* Thread function for all Active Objects (uC/OS-II task signature) */
static void Active_eventLoop(void *pdata)
{
    Active *me = (Active *)pdata; /* The AO instance "me" */
    
    /* Initialize the AO */
    static Event const initEvt = { INIT_SIG };
    (*me->dispatch)(me, &initEvt);
    
    /* Event loop ("Message pump") */
    while (1)
    {
        Event *e; /* Pointer to event object ("message") */
        INT8U err;
        
        /* Wait for any event and receive it into object 'e' */
        e = OSQPend(me->queue, 0U, &err); /* BLOCKING! */
        Q_ASSERT(err == 0U);
        
        /* Dispatch event to the active object 'me' */
        (*me->dispatch)(me, e); /* NO BLOCKING! */
    }
}

void Active_start(Active * const me,
                  INT8U prio, /* priority (1-based) */
                  Event **queueSto,
                  INT32U queueLen,
                  void *stackSto,
                  INT32U stackSize,
                  INT16U opt)
{
    INT8U err;
    OS_STK *stk_sto = stackSto;
    INT32U stk_depth = (stackSize / sizeof(OS_STK));
    
    /* Precondition */
    Q_ASSERT(me /* AO instance must be provided (cannot be NULL) */
             && (0 < prio) && (prio < OS_LOWEST_PRIO - 2U));
    
    me->queue = OSQCreate((void **)queueSto, queueLen);
    Q_ASSERT(me->queue); /* Queue must be created */
    
    me->thread = OS_LOWEST_PRIO - 2U - prio; /* uC/OS-II priority */
    err = OSTaskCreateExt(
            &Active_eventLoop,          /* The thread function */
            me,                         /* The 'pdata' parameter */
            &stk_sto[stk_depth - 1],    /* ptos */
            me->thread,                 /* uC/OS-II priority */
            prio,                       /* Task ID (Unique AO priority) */
            stk_sto,                    /* pbos */
            stk_depth,                  /* Stack depth */
            (void *)0,                  /* pext */
            opt);                       /* Task options */
    Q_ASSERT(err == 0U); /* Thread must be created */
}

void Active_post(Active * const me, Event const * const e)
{
    OSQPost(me->queue, (void *)e);
}

/*--------------------------------------------------------------------------*/
/* Time Event services... */

static TimeEvent *l_tevt[10];   /* All TimeEvents in the application */
static INT8U l_tevtNum;         /* Current number of TimeEvents */

void TimeEvent_ctor(TimeEvent * const me, Signal sig, Active *act)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif

    /* Not critical section because it is presumed that all TimeEvents
     * are created *before* multitasking has started.
     */
    me->super.sig = sig;
    me->act = act;
    me->timeout = 0U;
    me->interval = 0U;
    
    /* Register one more TimeEvent instance */
    OS_ENTER_CRITICAL();
    Q_ASSERT(l_tevtNum < sizeof(l_tevt)/sizeof(l_tevt[0]));
    l_tevt[l_tevtNum] = me;
    ++l_tevtNum;
    OS_EXIT_CRITICAL();
}

void TimeEvent_arm(TimeEvent * const me, INT32U timeout, INT32U interval)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif
    OS_ENTER_CRITICAL();
    me->timeout = timeout;
    me->interval = interval;
    OS_EXIT_CRITICAL();
}

void TimeEvent_disarm(TimeEvent * const me)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif
    OS_ENTER_CRITICAL();
    me->timeout = 0;
    OS_EXIT_CRITICAL();
}

void TimeEvent_tick(void)
{
    INT8U i;
    for (i = 0U; i < l_tevtNum; ++i)
    {
        TimeEvent * const t = l_tevt[i];
        Q_ASSERT(t); /* TimeEvent instance must be registered */
        if (t->timeout > 0U) /* Is this TimeEvent armed */
        {
            if (--t->timeout == 0U) /* Is it expiring now? */
            {
                Active_post(t->act, &t->super);
                t->timeout = t->interval; /* rearm or disarm (one-shot) */
            }
        }
    }
}
