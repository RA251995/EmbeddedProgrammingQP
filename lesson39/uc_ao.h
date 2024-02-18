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
#ifndef __UC_AO_H
#define __UC_AO_H

#include "ucos_ii.h"    /* uC/OS-II API, port and compile-time configuration */

/*---------------------------------------------------------------------------*/
/* Event Facilties */
typedef INT16U Signal; /* Event signal */

enum ReservedSignals
{
    INIT_SIG, /* Dispactched to AO before entering event-loop */
    ENTRY_SIG,
    EXIT_SIG,
    USER_SIG  /* First signal available to users */
};

/* Event base class */
typedef struct
{
    Signal sig; /* Event signal */
    /* Event parameters added in subclasses of Event */
} Event;

/*---------------------------------------------------------------------------*/
/* Finite State Machine facilities */

typedef struct Fsm Fsm;

typedef enum { TRANS_STATUS, HANDLED_STATUS, IGNORED_STATUS, INIT_STATUS } State;

typedef State (*StateHandler)(Fsm * const me, Event const * const e);

#define TRAN(target_) (((Fsm *)me)->state = (StateHandler)(target_), TRANS_STATUS)

struct Fsm
{
    StateHandler state; /* The "state" variable */
};

void Fsm_ctor(Fsm * const me, StateHandler initial);
void Fsm_init(Fsm * const me, Event const * const e);
void Fsm_dispatch(Fsm * const me, Event const * const e);

/*---------------------------------------------------------------------------*/
/* Active Object facilities */

typedef struct Active Active; /* Forward declaration */

typedef void (*DispatchHandler)(Active * const me, Event const * const e);

/* Active object base class */
struct Active
{
    Fsm super;          /* Inherit Fsm */
    INT8U thread;       /* Private thread (the unique uC/OS-II task priority */
    OS_EVENT *queue;    /* Private message queue */

    /* Active object data added in subclasses of Active */
};

void Active_ctor(Active * const me, StateHandler dispatch);
void Active_start(Active * const me,
                  INT8U prio, /* priority (1-based) */
                  Event **queueSto,
                  INT32U queueLen,
                  void *stackSto,
                  INT32U stackSize,
                  INT16U opt);
void Active_post(Active * const me, Event const * const e);

/*---------------------------------------------------------------------------*/
/* Time Event facilities */

/* Time Event class */
typedef struct
{
    Event super;        /* Inherit Event */
    Active *act;        /* The AO that requested this TimeEvent */
    INT32U timeout;     /* Timeout counter; 0 means not armed */
    INT32U interval;    /* Interval for periodic functions, 0 means one-shot */
} TimeEvent;

void TimeEvent_ctor(TimeEvent * const me, Signal sig, Active *act);
void TimeEvent_arm(TimeEvent * const me, INT32U timeout, INT32U interval);
void TimeEvent_disarm(TimeEvent * const me);

/* static (i.e., class-wide) operation */
void TimeEvent_tick(void);

#endif /* __UC_AO_H */