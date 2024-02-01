#ifndef __MIROS_H
#define __MIROS_H

#include <stdint.h>

/* Thread Control Block (TCB) */
typedef struct
{
    void *sp; /* Stack pointer */
    uint32_t timeout; /* Timeout delay down-counter */
    uint8_t priority; /* Thread priority */
} OSThread;

typedef void (*OSThreadHandler)(void);

void OS_init(void *stackAry, uint32_t stackSize);

/* Callback to handle the idle condition */
void OS_onIdle(void);

/* Transfer control to the RTOS to run the threads */
void OS_run(void);

/* Process all timeouts */
void OS_tick(void);

/* Blocking delay */
void OS_delay(uint32_t ticks);

/* Callback to configure and enable interrupts */
void OS_onStartup(void);

/* This function must always be called with interrupts disabled */
void OS_sched(void);

/* stackSize in bytes */
void OSThread_start(
    OSThread *me,
    uint8_t priority,
    OSThreadHandler threadHandler,
    void *stackAry, uint32_t stackSize 
);

#endif /* __MIROS_H */
