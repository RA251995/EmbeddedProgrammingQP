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

/* This function must always be called with interrupts disabled */
void OS_sched(void);

/* stackSize in bytes */
void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void *stackAry, uint32_t stackSize 
);

#endif /* __MIROS_H */
