#ifndef __MIROS_H
#define __MIROS_H

#include <stdint.h>

/* Thread Control Block (TCB) */
typedef struct
{
    void *sp;
    
} OSThread;

typedef void (*OSThreadHandler)(void);

void OS_init(void);

/* Transfer control to the RTOS to run the threads */
void OS_run(void);

/* Callback to configure and enable interrupts */
void OS_onStartup(void);

/* This function must always be called with interrupts disabled */
void OS_sched(void);

/* stackSize in bytes */
void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void *stackAry, uint32_t stackSize 
);

#endif /* __MIROS_H */
